#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>

class TunInterface {
public:
  TunInterface(const char *name, bool is_tap, bool no_pi): fd_(open("/dev/net/tun", O_RDWR)) {
    if (fd_ < 0) {
      std::cerr << "Error\n";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    if (is_tap) {
      ifr.ifr_flags = IFF_TAP;
    } else {
      ifr.ifr_flags = IFF_TUN;
    }

    if (no_pi) {
      ifr.ifr_flags |= IFF_NO_PI;
    }

    if (name != NULL) {
      strncpy(ifr.ifr_name, name, strlen(name));
    }

    if (ioctl(fd_, TUNSETIFF, (void *) &ifr) < 0) {
      std::cerr << "Error construct\n";
      std::cerr << strerror(errno) << std::endl;
      exit(1);
    }

  }
  
  void Start() {
    char buf[1024] = {0};
    ssize_t n;
    while (n = read(fd_, buf, sizeof(buf))) {
      if (n < 0) {
        std::cerr << "Error start\n";
        std::cerr << strerror(errno) << std::endl;
      }
      std::cout << n << std::endl;
      std::cout << buf << std::endl;
      memset(buf, 0, sizeof(buf));
    }
  }  

private:
  const int fd_;
};

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Usage: tun_interface <name>\n";
    exit(1); 
  }

  try {
    TunInterface tun_interface(argv[1], false, false);
    tun_interface.Start();
  } catch (std::exception& e) {
    std::cerr << "Error main\n";
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
