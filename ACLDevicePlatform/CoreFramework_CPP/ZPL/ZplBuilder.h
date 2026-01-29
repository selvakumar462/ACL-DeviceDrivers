#pragma once
#include <string>
#include <vector>
#include <memory>

namespace ACLDevicePlatform {
namespace Core {
namespace ZPL {

    /**
     * @brief ZPL (ACL Programming Language) label builder
     * 
     * Provides functionality to create ZPL commands for label printing.
     * Supports text, barcodes, images, and other ZPL elements.
     */
    class ZplBuilder {
    public:
        ZplBuilder();
        ~ZplBuilder();

        /**
         * @brief Set label dimensions
         * @param width Label width in dots
         * @param height Label height in dots
         * @param gap Gap between labels in dots
         * @return Reference to this builder for chaining
         */
        ZplBuilder& SetLabelDimensions(int width, int height, int gap = 0);

        /**
         * @brief Set label origin
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @return Reference to this builder for chaining
         */
        ZplBuilder& SetOrigin(int x, int y);

        /**
         * @brief Add text to the label
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param font Font type (0-9)
         * @param height Font height in dots
         * @param width Font width in dots
         * @param text Text to print
         * @param rotation Rotation angle (0, 90, 180, 270)
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddText(int x, int y, int font, int height, int width, 
                           const std::string& text, int rotation = 0);

        /**
         * @brief Add barcode to the label
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param barcodeType Barcode type (CODE128, CODE39, etc.)
         * @param height Barcode height in dots
         * @param printInterpretationLine Whether to print human-readable text
         * @param text Barcode data
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddBarcode(int x, int y, const std::string& barcodeType, 
                              int height, bool printInterpretationLine, 
                              const std::string& text);

        /**
         * @brief Add a box to the label
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param width Box width in dots
         * @param height Box height in dots
         * @param borderThickness Border thickness in dots
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddBox(int x, int y, int width, int height, int borderThickness = 1);

        /**
         * @brief Add a line to the label
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param length Line length in dots
         * @param thickness Line thickness in dots
         * @param direction Direction (H for horizontal, V for vertical)
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddLine(int x, int y, int length, int thickness, char direction = 'H');

        /**
         * @brief Add a field block (for multi-line text)
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param width Block width in dots
         * @param lines Number of lines
         * @param text Text content
         * @param alignment Text alignment (L, C, R, J). Default is 'L'.
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddFieldBlock(int x, int y, int width, int lines, const std::string& text, char alignment = 'L');

        /**
         * @brief Add a graphic box (filled rectangle)
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param width Box width in dots
         * @param height Box height in dots
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddGraphicBox(int x, int y, int width, int height);

        /**
         * @brief Add a reverse video text (white text on black background)
         * @param x X coordinate in dots
         * @param y Y coordinate in dots
         * @param font Font type
         * @param height Font height in dots
         * @param width Font width in dots
         * @param text Text to print
         * @return Reference to this builder for chaining
         */
        ZplBuilder& AddReverseText(int x, int y, int font, int height, int width, const std::string& text);

        /**
         * @brief Build the complete ZPL label
         * @return Complete ZPL command string
         */
        std::string Build() const;

        /**
         * @brief Clear all current label content
         * @return Reference to this builder for chaining
         */
        ZplBuilder& Clear();

        /**
         * @brief Set the print quantity
         * @param quantity Number of labels to print
         * @return Reference to this builder for chaining
         */
        ZplBuilder& SetQuantity(int quantity);

        /**
         * @brief Set the print density (DPI)
         * @param density Print density (6, 8, 12, 24 for common ACL printers)
         * @return Reference to this builder for chaining
         */
        ZplBuilder& SetDensity(int density);

    private:
        struct Command {
            std::string content;
        };

        std::vector<Command> m_commands;
        int m_width;
        int m_height;
        int m_gap;
        int m_quantity;
        int m_density;
        int m_currentX;
        int m_currentY;

        /**
         * @brief Add a raw ZPL command
         * @param command ZPL command string
         */
        void AddCommand(const std::string& command);

        /**
         * @brief Validate coordinates are within label bounds
         * @param x X coordinate
         * @param y Y coordinate
         * @return true if valid, false otherwise
         */
        /**
         * @brief Validate coordinates are within label bounds
         * @param x X coordinate
         * @param y Y coordinate
         * @return true if valid, false otherwise
         */
        bool ValidateCoordinates(int x, int y) const;

        /**
         * @brief Sanitize text for ZPL to avoid control character conflicts
         * @param input Raw input text
         * @return Sanitized text with hex escaping for special characters
         */
        std::string Sanitize(const std::string& input) const;
    };

    /**
     * @brief Smart pointer type for ZPL builder
     */
    using ZplBuilderPtr = std::unique_ptr<ZplBuilder>;

} // namespace ZPL
} // namespace Core
} // namespace ACLDevicePlatform