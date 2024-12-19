//
// Created by Hayden Rivas on 12/7/24.
//

#ifndef SLATEEDITOR_IMANAGER_H
#define SLATEEDITOR_IMANAGER_H

namespace Slate {
    class IManager {
    public:
        IManager() = default;
        virtual ~IManager() = default;
    private:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    };
}

#endif //SLATEEDITOR_IMANAGER_H
