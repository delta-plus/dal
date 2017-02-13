//////////////////////////////////////////////////////////////////////////////////                                                                            //
//                          Desktop Animator for Linux                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <fastformat/fastformat.hpp>
#include <fastformat/internal/initialiser.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char* argv[]) {

  if (argc < 4) {

    cout << "Usage: daw [DIRECTORY OF IMAGES]\n           [TIME BETWEEN FRAMES IN MICROSECONDS]\n           [FILE EXTENSION OF IMAGES]\n\nNote: Must include a forward slash at end of specified directory name and a \".\" before file extension." << endl;

    cin.get();
    return -1;
  }

  DIR *direc = NULL;
  const char *images = argv[1];
  direc = opendir(images);
  struct dirent *dirlist = NULL;
  std::stringstream ss;
  string framestr;
  vector<string> frames(1), checkedframes(1), goodframes(1);
  string currentframe;
  const char *extension = argv[3];
  const char *framecheck1, *framecheck2;
  string framenumber;
  pid_t process;
  int useconds = atoi(argv[2]);
  
  if (direc == NULL) {
    return -1;
  }

  while ((dirlist = readdir(direc)) != NULL) {
    ss.str(dirlist->d_name);
    framestr = ss.str();
    frames.push_back(framestr);
    frames.reserve(1);
  }

  for (int i = 0; i < frames.size(); ++i) {
    if (frames[i].find(extension) != std::string::npos) {
      checkedframes.push_back(frames[i]);
      checkedframes.reserve(1);
    }
  }
  
  for (int j = 0; j < checkedframes.size(); ++j) {
    framenumber = "";
    fastformat::write(framenumber, "frame_", j, "_");
    for (int k = 0; k < checkedframes.size(); ++k) {
      if (checkedframes[k].find(framenumber) != std::string::npos) {
        goodframes.push_back(checkedframes[k]);
        goodframes.reserve(1);
      }
    }
  }



  for (int l = 0; l < goodframes.size(); ++l) {
    currentframe = "";
    fastformat::write(currentframe, "file://", argv[1], goodframes[l]);
    if ((process = fork()) == 0) {
      execl("/usr/bin/gsettings", "gsettings", "set", "org.gnome.desktop.background", "picture-uri", currentframe.c_str(), NULL);
    }

    usleep(useconds);
    }

  return 0;
}
