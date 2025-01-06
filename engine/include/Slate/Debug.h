//
// Created by Hayden Rivas on 12/24/24.
//

#pragma once

#include <cstdio>
#include <csignal>

namespace Slate {
	inline const char* ErrorToString(GLenum errorcode) {
		switch (errorcode) {
			case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
			case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
			case GL_INVALID_INDEX: return "GL_INVALID_INDEX";
			case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
			case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
			default: return "Unknown Error Code";
		}
	}

    inline void CheckOpenGLError(const char *stmt, const char *fname, int line) {
        unsigned int err = glGetError();
        if (err != GL_NO_ERROR) {
            fprintf(stderr, "OPENGL: %s:%i -> OpenGL Error: %x (%s) ->\n\t %s\n", fname, line, err, ErrorToString(err), stmt);
            raise(SIGABRT);
        }
    }
}
#ifdef DEBUG
#define GL_CALL(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CALL(stmt) stmt
#endif



