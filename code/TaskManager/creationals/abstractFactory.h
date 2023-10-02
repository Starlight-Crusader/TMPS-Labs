#include "../io/printer.h"
#include "../io/scanner.h"


class IOFactory {
    public:
        virtual Printer* getPrinter() = 0;
        virtual Scanner* getScanner() = 0;
};

class ConsoleIOFactory : public IOFactory {
    public:
        Printer* getPrinter() override { return new ConsolePrinter(); }
        Scanner* getScanner() override { return new ConsoleScanner(); }
};

class FileIOFactory : public IOFactory {
    public:
        Printer* getPrinter() override { return new FilePrinter(); }
        Scanner* getScanner() override { return new FileScanner(); }
};