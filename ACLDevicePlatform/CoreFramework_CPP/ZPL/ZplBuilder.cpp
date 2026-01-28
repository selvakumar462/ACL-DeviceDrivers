#include "ZplBuilder.h"
#include <sstream>
#include <iomanip>

namespace ACLDevicePlatform {
namespace Core {
namespace ZPL {

    ZplBuilder::ZplBuilder() 
        : m_width(400), m_height(600), m_gap(10), m_quantity(1), m_density(8), 
          m_currentX(0), m_currentY(0) {
    }

    ZplBuilder::~ZplBuilder() {
    }

    ZplBuilder& ZplBuilder::SetLabelDimensions(int width, int height, int gap) {
        m_width = width;
        m_height = height;
        m_gap = gap;
        return *this;
    }

    ZplBuilder& ZplBuilder::SetOrigin(int x, int y) {
        m_currentX = x;
        m_currentY = y;
        return *this;
    }

    ZplBuilder& ZplBuilder::AddText(int x, int y, int font, int height, int width, 
                                   const std::string& text, int rotation) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^A" << static_cast<char>('A' + font) 
            << "," << height << "," << width;
        
        if (rotation != 0) {
            oss << ",," << rotation;
        }
        
        oss << "^FD" << text << "^FS";
        AddCommand(oss.str());
        return *this;
    }

    ZplBuilder& ZplBuilder::AddBarcode(int x, int y, const std::string& barcodeType, 
                                      int height, bool printInterpretationLine, 
                                      const std::string& text) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^B" << barcodeType;
        
        if (printInterpretationLine) {
            oss << "N";
        } else {
            oss << "N";
        }
        
        oss << "," << height << ",Y,N";
        AddCommand(oss.str());
        AddCommand("^FD" + text + "^FS");
        return *this;
    }

    ZplBuilder& ZplBuilder::AddBox(int x, int y, int width, int height, int borderThickness) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^GB" << width << "," << height << "," << borderThickness << "^FS";
        AddCommand(oss.str());
        return *this;
    }

    ZplBuilder& ZplBuilder::AddLine(int x, int y, int length, int thickness, char direction) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y;
        
        if (direction == 'H' || direction == 'h') {
            oss << "^GB" << length << "," << thickness << ",1^FS";
        } else {
            oss << "^GB" << thickness << "," << length << ",1^FS";
        }
        
        AddCommand(oss.str());
        return *this;
    }

    ZplBuilder& ZplBuilder::AddFieldBlock(int x, int y, int width, int lines, const std::string& text) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^FB" << width << "," << lines << ",0,L,1" 
            << "^FD" << text << "^FS";
        AddCommand(oss.str());
        return *this;
    }

    ZplBuilder& ZplBuilder::AddGraphicBox(int x, int y, int width, int height) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^GB" << width << "," << height << "," << height << ",B^FS";
        AddCommand(oss.str());
        return *this;
    }

    ZplBuilder& ZplBuilder::AddReverseText(int x, int y, int font, int height, int width, const std::string& text) {
        std::ostringstream oss;
        oss << "^FO" << x << "," << y 
            << "^AF" << "," << height << "," << width 
            << "^FD" << text << "^FS";
        AddCommand(oss.str());
        return *this;
    }

    std::string ZplBuilder::Build() const {
        std::ostringstream oss;
        
        // Start label
        oss << "^XA";
        
        // Set label configuration
        oss << "^PW" << m_width << "^LL" << m_height;
        if (m_gap > 0) {
            oss << "^GAP" << m_gap;
        }
        
        // Set print density
        oss << "^MNM" << m_density;
        
        // Set quantity
        oss << "^PQ" << m_quantity;
        
        // Add all commands
        for (const auto& cmd : m_commands) {
            oss << cmd.content;
        }
        
        // End label
        oss << "^XZ";
        
        return oss.str();
    }

    ZplBuilder& ZplBuilder::Clear() {
        m_commands.clear();
        return *this;
    }

    ZplBuilder& ZplBuilder::SetQuantity(int quantity) {
        m_quantity = quantity;
        return *this;
    }

    ZplBuilder& ZplBuilder::SetDensity(int density) {
        m_density = density;
        return *this;
    }

    void ZplBuilder::AddCommand(const std::string& command) {
        m_commands.push_back({command});
    }

    bool ZplBuilder::ValidateCoordinates(int x, int y) const {
        return (x >= 0 && x <= m_width && y >= 0 && y <= m_height);
    }

} // namespace ZPL
} // namespace Core
} // namespace ACLDevicePlatform
