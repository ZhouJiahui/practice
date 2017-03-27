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
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <algorithm>

class TunInterface {
public:
  TunInterface(const char *name, bool is_tap, bool no_pi): fd_(open("/dev/net/tun", O_RDWR)),
                                                           is_tap_(is_tap),
                                                           no_pi_(no_pi) {
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
    if (is_tap_) {
      ifr.ifr_flags = IFF_TAP;
    } else {
      ifr.ifr_flags = IFF_TUN;
    }

    if (no_pi_) {
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
      struct tun_pi * pi = (struct tun_pi *) buf;
      if (pi->flags & TUN_PKT_STRIP) {
        std::cerr << "Receive buffer is too small\n"; 
        continue;
      }
      if (pi->proto == htons(ETH_P_IP)) {
        iphdr *ip_header = reinterpret_cast<iphdr*>(buf + sizeof(tun_pi));
        std::swap(ip_header->saddr, ip_header->daddr);
        udphdr *udp_header = reinterpret_cast<udphdr*>(buf + sizeof(tun_pi) + sizeof(iphdr));
        std::swap(udp_header->source, udp_header->dest);
        size_t s = sizeof(struct tun_pi) + sizeof(struct iphdr) + sizeof(struct udphdr);
        const char *data = buf  + s;
        std::cout << n - s << std::endl;
        std::cout << data << std::endl;
        n = write(fd_, buf, n);
        memset(buf, 0, sizeof(buf));
      }
    }
  }  

private:
  const int fd_;
  bool is_tap_;
  bool no_pi_;
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
