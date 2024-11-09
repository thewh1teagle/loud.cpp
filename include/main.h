#include <whisper.h>

#ifndef COMMIT_HASH
#define COMMIT_HASH ""
#endif

// https://stackoverflow.com/a/68523164
#define __FILENAME__ strstr(__FILE__, SOURCE_PATH) + SOURCE_PATH_SIZE

#ifndef CHECK_NULL
#define CHECK_NULL(ptr)                                                        \
  if (!ptr) {                                                                  \
    std::cerr << "Error: Null pointer detected (" << #ptr << ")"               \
              << " in " << __func__ << " at " << __FILENAME__ << ":"           \
              << __LINE__ << std::endl;                                        \
    if (COMMIT_HASH[0] != '\0') {                                              \
      std::cerr << "  Commit: " << COMMIT_HASH << std::endl;                   \
      std::cerr << "  See the issue report here (including commit hash):"      \
                << std::endl;                                                  \
      std::cerr << "  "                                                        \
                   "https://github.com/thewh1teagle/loud.cpp/issues/"          \
                   "new?body=commit="                                          \
                << COMMIT_HASH << std::endl;                                   \
    } else {                                                                   \
      std::cerr << "Does this seem unexpected? Report the issue at: "          \
                << "https://github.com/thewh1teagle/loud.cpp/issues/new"       \
                << std::endl;                                                  \
    }                                                                          \
    return EXIT_FAILURE;                                                       \
  }
#endif

static void cb_log_disable(enum ggml_log_level, const char *, void *) {}