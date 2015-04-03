/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2014 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#if defined(SVG_RENDERER)

// mapnik
#include <mapnik/svg/output/svg_renderer.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/text/symbolizer_helpers.hpp>
#include <mapnik/text/glyph_info.hpp>
#include <mapnik/text/glyph_positions.hpp>
#include <mapnik/text/text_properties.hpp>
#include <mapnik/unicode.hpp>

#include <mapnik/svg/geometry_svg_generator_impl.hpp>
#include <mapnik/svg/output/svg_output_attributes.hpp>
#include <mapnik/svg/output/svg_output_grammars.hpp>

#include <mapnik/transform_path_adapter.hpp>
#include <mapnik/symbolizer_dispatch.hpp>

// boost
#include <boost/spirit/include/karma.hpp>

namespace mapnik
{
/*!
 * @brief Collect presentation attributes found in text symbolizer.
 */

// extends a curve at both endpoints such that the resulting curve
// is smooth at the endpoints and hopefully matches the profile of the curve
// in the background. this needs to be done because the path outputted
// by the shapers does not always fit the text when rendered not by mapnik
void extend_curve(int num_pts, std::deque<double> &x, std::deque<double> &y) {
    if(x.size() > 1 && y.size() > 1) {
        double dx1 = x[0]-x[1], dy1 = y[0]-y[1];
        for(int i = 0; i < num_pts; i++) {
            x.push_front(x[0]+dx1*(i+1));
            y.push_front(y[0]+dy1*(i+1));
        }

        int len = x.size();
        double dx2 = x[len-1]-x[len-2], dy2 = y[len-1]-y[len-2];
        for(int i = 0; i < num_pts; i++) {
            x.push_back(x[len-1]+dx2*(i+1));
            y.push_back(y[len-1]+dy2*(i+1));
        }
    }
}

template <typename OutputIterator, typename PathType>
void generate_text_on_path(OutputIterator & output_iterator, value_unicode_string const& str, svg::text_output_attributes_fill const &text_attr,
                           svg::text_output_attributes_stroke const &text_halo_attr, PathType const &path,
                           svg::path_output_attributes const &attr)
{
    // assign some id to the path
    int path_id = rand();
    boost::spirit::karma::lit_type lit;
    boost::spirit::karma::int_type int_;

    // create path definition
    boost::spirit::karma::generate(output_iterator, lit("<defs>\n\t") << lit("<path id=\"p") << int_ << lit("\" "), path_id);

    static const svg::svg_path_generator<OutputIterator,PathType> svg_path_grammer;
    boost::spirit::karma::generate(output_iterator, svg_path_grammer, path);
    
    static const svg::svg_path_attributes_grammar<OutputIterator> attributes_grammar;
    boost::spirit::karma::generate(output_iterator, lit(" ") << attributes_grammar << lit("/>\n</defs>\n"), attr);

    // convert text to utf as this is the default xml encoding
    std::string utf8;
    to_utf8(str, utf8);

    // default value of stroke in svg is 1, so do not output a halo if the radius is 1
    static const svg::svg_text_stroke_attributes_grammar<OutputIterator> halo_grammar;
    if(text_halo_attr.halo_radius() != 1.0) {
        boost::spirit::karma::generate(output_iterator, lit("<text ") << halo_grammar << ">", text_halo_attr);
        boost::spirit::karma::generate(output_iterator, lit("\n<textPath xlink:href=\"#p") << int_ << lit("\">"), path_id);
        boost::spirit::karma::generate(output_iterator, utf8 << lit("</textPath>\n</text>\n"));
    }
    static const svg::svg_text_fill_attributes_grammar<OutputIterator> text_grammar;
    boost::spirit::karma::generate(output_iterator, lit("<text ") << text_grammar << ">", text_attr);
    boost::spirit::karma::generate(output_iterator, lit("\n<textPath xlink:href=\"#p") << int_ << lit("\">"), path_id);
    boost::spirit::karma::generate(output_iterator, utf8 << lit("</textPath>\n</text>\n"));
}

template <typename T>
void svg_renderer<T>::process(text_symbolizer const& sym,
                                  mapnik::feature_impl & feature,
                                  proj_transform const& prj_trans)
{
    agg::trans_affine tr;
    auto transform = get_optional<transform_type>(sym, keys::geometry_transform);
    if (transform) evaluate_transform(tr, feature, common_.vars_, *transform, common_.scale_factor_);
    text_symbolizer_helper helper(
            sym, feature, common_.vars_, prj_trans,
            common_.width_, common_.height_,
            common_.scale_factor_,
            common_.t_, common_.font_manager_, *common_.detector_,
            common_.query_extent_, tr);
            
    svg::path_output_attributes line_attr;

    placements_list const &placements = helper.get();
        
    if(placements.size() > 0 && placements.front()->size() > 0){
        detail::evaluated_format_properties const& text_format = *(placements.front()->begin()->glyph.format);
        //auto const& tglyph_pos = (*tglyphs).front();

        // Font settings
        // halo effect is emulated by having 2 copies of the
        // text. one with fill only, one with stroke only
        // it can be done with filters but there are libraries
        // that don't support filter (especially in android)
        svg::text_output_attributes_fill text_attr;
//        text_attr.set_text_ratio(sym.get_text_ratio());
        text_attr.set_face_name(text_format.face_name);
        if(text_format.fontset)
            text_attr.set_fontset(text_format.fontset->get_name());
        text_attr.set_text_size(text_format.text_size);
        text_attr.set_fill(text_format.fill);
        text_attr.set_char_spacing(text_format.character_spacing);
//        text_attr.set_text_transform(get<transform_type>(sym, keys::text_transform));

        // the halo duplicate of the text
        svg::text_output_attributes_stroke halo_attr;
//        halo_attr.set_text_ratio(sym.get_text_ratio());
        halo_attr.set_face_name(text_format.face_name);
        if(text_format.fontset) {
            halo_attr.set_fontset(text_format.fontset->get_name());
        }
        halo_attr.set_text_size(text_format.text_size);
        halo_attr.set_halo_fill(text_format.halo_fill);
        // default stroke width in svg is 1.0
        halo_attr.set_halo_radius(1.0+text_format.halo_radius);
        halo_attr.set_char_spacing(get<double>(sym, keys::spacing, 1.0));
//        text_attr.set_text_transform(get<transform_type>(sym, keys::halo_transform));


        composite_mode_e comp_op = get<composite_mode_e>(sym, keys::comp_op, feature, common_.vars_, src_over);
        composite_mode_e halo_comp_op = get<composite_mode_e>(sym, keys::halo_comp_op, feature, common_.vars_, src_over);
        double opacity = get<double>(sym,keys::opacity,feature, common_.vars_, 1.0);

        for (glyph_positions_ptr glyphs : placements)
        {
            std::deque<double> x,y;
            pixel_position base = glyphs->get_base_point();
            base.y = common_.height_-base.y;
            value_unicode_string curr;
        
            for (auto const& glyph_pos : *glyphs)
            {
                curr = *(glyph_pos.glyph.string_value.get());
            
                // determine position of character in the string
                pixel_position pos = glyph_pos.pos+glyph_pos.glyph.offset.rotate(glyph_pos.rot)+base;
            
                // translate to svg coordinate space
                x.push_back(pos.x);
                y.push_back(common_.height_-pos.y);
            }
        
            if(x.size() > 0){
                // create geometry from the accumulated positions
                extend_curve(1, x, y);
                geometry_type geom(geometry_type::LineString);
                for(int i = 0; i < x.size(); i++) {
                    if(i == 0)
                        geom.move_to(x[i], y[i]);
                    else
                        geom.line_to(x[i], y[i]);
                }
                
                generate_text_on_path(generator_.output_iterator_, curr, text_attr, halo_attr, geom, line_attr);
                x.clear(); y.clear();
            }
        }
    }
}

template void svg_renderer<std::ostream_iterator<char> >::process(text_symbolizer const&,
                                                                  mapnik::feature_impl &,
                                                                  proj_transform const&);
}

#endif
