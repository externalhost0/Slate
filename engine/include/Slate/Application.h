//
// Created by Hayden Rivas on 11/27/24.
//

#ifndef SLATEEDITOR_APPLICATION_H
#define SLATEEDITOR_APPLICATION_H

namespace Slate {
    class Application {
    public:
        static Application& Get() { return *instance; };
        virtual ~Application() = default;
    private:
        static Application* instance;
    };
}

#endif //SLATEEDITOR_APPLICATION_H
