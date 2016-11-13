/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <tmxlite/Object.hpp>
#include <tmxlite/pugixml.hpp>

#include <sstream>

using namespace tmx;

Object::Object()
    : m_UID     (0),
    m_rotation  (0.f),
    m_tileID    (0),
    m_visible   (true),
    m_shape     (Shape::Rectangle)
{

}

//public
void Object::parse(const pugi::xml_node& node)
{
    std::string attribString = node.name();
    if (attribString != "object")
    {
        Logger::log("This not an Object node, parsing skipped.", Logger::Type::Error);
        return;
    }

    m_UID = node.attribute("id").as_int();
    m_name = node.attribute("name").as_string();
    m_type = node.attribute("type").as_string();
    m_position.x = node.attribute("x").as_float();
    m_AABB.left = m_position.x;
    m_position.y = node.attribute("y").as_float();
    m_AABB.top = m_position.y;
    m_AABB.width = node.attribute("width").as_float();
    m_AABB.height = node.attribute("height").as_float();
    m_rotation = node.attribute("rotation").as_float();
    m_tileID = node.attribute("gid").as_int();
    m_visible = node.attribute("visible").as_bool();

    for (const auto& child : node.children())
    {
        attribString = child.name();
        if (attribString == "properties")
        {
            for (const auto& p : child.children())
            {
                m_properties.emplace_back();
                m_properties.back().parse(p);
            }
        }
        else if (attribString == "ellipse")
        {
            m_shape = Shape::Ellipse;
        }
        else if (attribString == "polygon")
        {
            m_shape = Shape::Polygon;
            parsePoints(child);
        }
        else if (attribString == "polyline")
        {
            m_shape = Shape::Polyline;
            parsePoints(child);
        }
    }
}

//private
void Object::parsePoints(const pugi::xml_node& node)
{
    if (node.attribute("points"))
    {
        std::string pointlist = node.attribute("points").as_string();
        std::stringstream stream(pointlist);
        std::vector<std::string> points;
        std::string pointstring;
        while (std::getline(stream, pointstring, ' '))
        {
            points.push_back(pointstring);
        }

        //parse each pair into sf::vector2f
        for (unsigned int i = 0; i < points.size(); i++)
        {
            std::vector<float> coords;
            std::stringstream coordstream(points[i]);

            float j;
            while (coordstream >> j)
            {
                coords.push_back(j);
                //TODO this should really ignore anything non-numeric
                if (coordstream.peek() == ',')
                {
                    coordstream.ignore();
                }
            }
            m_points.emplace_back(coords[0], coords[1]);
        }
    }
    else
    {
        Logger::log("Points for polygon or polyline object are missing", Logger::Type::Warning);
    }
}