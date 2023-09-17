#include "../io/printer.h"
#include "../io/scanner.h"


class IOFactory {
    public:
        virtual Printer* getPrinter() = 0;
        virtual Scanner* getScanner() = 0;
};

class ConsoleIOFactory : public IOFactory {
    public:
        Printer* getPrinter() { return new ConsolePrinter(); }
        Scanner* getScanner() { return new ConsoleScanner(); }
};

class FileIOFactory : public IOFactory {
    public:
        Printer* getPrinter() { return new FilePrinter(""); }
        Scanner* getScanner() { return new FileScanner(); }
};