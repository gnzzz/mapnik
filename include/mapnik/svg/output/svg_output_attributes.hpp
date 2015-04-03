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

#ifndef MAPNIK_SVG_OUTPUT_ATTRIBUTES
#define MAPNIK_SVG_OUTPUT_ATTRIBUTES

// mapnik
#include <mapnik/color.hpp>
#include <mapnik/symbolizer_base.hpp>
#include <mapnik/symbolizer_enumerations.hpp>

// stl
#include <string>

namespace mapnik { namespace svg {

    /*!
     * @brief SVG path tag attributes.
     * This structure encapsulates the values needed to
     * generate a path tag. It is meant to be filled with
     * the values stored in line_ and polygon_ symbolizers.
     *
     * The values are stored using the variable types that
     * are required for output generation, but the interface
     * is written with the original types. "set" methods
     * perform the necessary conversions (i.e. from color to
     * hex string).
     */
    struct path_output_attributes
    {
        path_output_attributes()
            : fill_color_("none"),
              fill_opacity_(1.0),
              stroke_color_("none"),
              stroke_opacity_(1.0),
              stroke_width_(0.0),
              stroke_linecap_("butt"),
              stroke_linejoin_("miter"),
              stroke_dasharray_(),
              stroke_dashoffset_(0.0)
        {}

        void set_fill_color(color const& fill_color);
        void set_fill_opacity(double fill_opacity);
        void set_stroke_color(color const& stroke_color);
        void set_stroke_opacity(double stroke_opacity);
        void set_stroke_width(double stroke_width);
        void set_stroke_linecap(line_cap_e stroke_linecap);
        void set_stroke_linejoin(line_join_e stroke_linejoin);
        void set_stroke_dasharray(dash_array const& stroke_dasharray);
        void set_stroke_dashoffset(double stroke_dashoffset);
        void set_path_id(std::string id);

        std::string const& fill_color() const;
        double fill_opacity() const;
        std::string const& stroke_color() const;
        double stroke_opacity() const;
        double stroke_width() const;
        std::string const& stroke_linecap() const;
        std::string const& stroke_linejoin() const;
        dash_array const& stroke_dasharray() const;
        double stroke_dashoffset() const;
        std::string const& path_id() const;

        /*!
         * @brief Set members back to their default values.
         */
        void reset();

        //private:
        // polygon symbolizer attributes.
        std::string fill_color_;
        double fill_opacity_;

        // line symbolizer attributes.
        std::string stroke_color_;
        double stroke_opacity_;
        double stroke_width_;
        std::string stroke_linecap_;
        std::string stroke_linejoin_;
        dash_array stroke_dasharray_;
        double stroke_dashoffset_;
        std::string path_id_;
    };

    /*!
     * @brief SVG rect tag attributes.
     * This structure encapsulates the values needed to
     * generate a rect tag. It is is meant to be used to
     * store background color information.
     *
     * The values are stored using the variable types that
     * are required for output generation, but the interface
     * is written with the original types. "set" methods
     * perform the necessary conversions (i.e. from color to
     * hex string
     */
    struct rect_output_attributes
    {
        rect_output_attributes()
            : x_(0),
              y_(0),
              width_(400),
              height_(400),
              fill_color_("#000000")
        {}

        rect_output_attributes(const int x, const int y, const unsigned width, const unsigned height, color const& fill_color)
            : x_(x),
              y_(y),
              width_(width),
              height_(height),
              fill_color_(fill_color.to_hex_string())
        {}

        void set_x(const int x);
        void set_y(const int y);
        void set_width(const unsigned width);
        void set_height(const unsigned height);
        void set_fill_color(color const& fill_color);

        int x() const;
        int y() const;
        int width() const;
        int height() const;
        std::string const& fill_color() const;

        /*!
         * @brief Set members back to their default values.
         */
        void reset();

        //private:
        int x_;
        int y_;
        unsigned width_;
        unsigned height_;
        std::string fill_color_;
    };

    /*!
     * @brief SVG root tag attributes.
     * This structure encapsulates the values needed to
     * generate an svg (root) tag.
     *
     * The values are stored using the variable types that
     * are required for output generation, but the interface
     * is written with the original types. "set" methods
     * perform the necessary conversions (i.e. from color to
     * hex string
     */
    struct root_output_attributes
    {
        root_output_attributes();

        root_output_attributes(const unsigned width, const unsigned height);

        void set_width(const unsigned width);
        void set_height(const unsigned height);
        void set_svg_version(double svg_version);
        void set_svg_namespace_url(std::string const& svg_namespace_url);

        unsigned width() const;
        unsigned height() const;
        double svg_version() const;
        std::string const& svg_namespace_url() const;

        /*!
         * @brief Set members back to their default values.
         */
        void reset();

        // SVG version to which the generated document will be compliant.
        static const double SVG_VERSION;
        // SVG XML namespace url.
        static const std::string SVG_NAMESPACE_URL;

        //private:
        unsigned width_;
        unsigned height_;
        double svg_version_;
        std::string svg_namespace_url_;
    };

    /*!
     * \brief The text_output_attributes struct
     * This structure encapsulates the values needed to
     * generate an svg (root) tag.
     *
     * The values are stored using the variable types that
     * are required for output generation, but the interface
     * is written with the original types. "set" methods
     * perform the necessary conversions (i.e. from color to
     * hex string
     */
    struct text_output_attributes
    {
        text_output_attributes()
            : text_ratio_(0),
              text_size_(10),
              char_spacing_(0),
              transform_("none")
        {;}

        // general layout options
        void set_text_ratio(const double ratio);
        // character formatting options
        void set_face_name(const std::string &name);
        void set_fontset(const std::string &name);
        void set_text_size(const double size);
        void set_char_spacing(const double &space);
        void set_text_transform(const text_transform_e &trans);

        double text_ratio() const;
        std::string face_name() const;
        std::string fontset() const;
        double text_size() const;
        double halo_radius() const;
        double char_spacing() const;
        std::string transform() const;

        // reset to default values
        void reset();

        // general layout options
        // - text-ratio
        // character formatting options
        // - face-name
        // - fontset-name
        // - size
        // - character spacing
        // - text-transform
        double text_ratio_;
        std::string face_name_;
        std::string fontset_;
        double text_size_;
        double char_spacing_;
        std::string transform_;
    };

    struct text_output_attributes_stroke : public text_output_attributes  {
        text_output_attributes_stroke()
            : text_output_attributes(),
              halo_fill_("#FFFFFF"),
              stroke_opacity_(1.0),
              halo_radius_(0)
        {;}

        // character formatting options
        void set_halo_fill(const color &fill);
        void set_halo_radius(const double &radius);

        std::string halo_fill() const;
        double stroke_opacity() const;
        double halo_radius() const;

        // reset to default values
        void reset();

        // character formatting options
        // - hallo-fill
        // - halo-radius
        std::string halo_fill_;
        double stroke_opacity_;
        double halo_radius_;
    };

    /*!
     * \brief The text_output_attributes struct
     * This structure encapsulates the values needed to
     * generate an svg (root) tag.
     *
     * The values are stored using the variable types that
     * are required for output generation, but the interface
     * is written with the original types. "set" methods
     * perform the necessary conversions (i.e. from color to
     * hex string
     */
    struct text_output_attributes_fill : public text_output_attributes
    {
        text_output_attributes_fill()
            : text_output_attributes(),
              fill_("#000000"),
              fill_opacity_(1.0)
        {;}

        // character formatting options
        void set_fill(const color &fill);

        std::string fill() const;
        double fill_opacity() const;

        // reset to default values
        void reset();

        // character formatting options
        // - fill
        std::string fill_;
        double fill_opacity_;
    };

    }}

#endif // MAPNIK_SVG_OUTPUT_ATTRIBUTES
