//
// Created by Hayden Rivas on 11/1/24.
//
#include <Slate/Application.h>
#include "Editor.h"

int main(int argc, char** argv){
    // make a Slate app
//    Slate::Application& editorApp = Slate::Application::Get();
    auto editorApp = Slate::Application::Create<Slate::Editor>();
    // now run it!
    editorApp->Run();
    return 0;
}