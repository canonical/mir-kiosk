#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include <cstdint>

#include <algorithm>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{
using GammaLUT = std::vector<uint16_t>;

GammaLUT make_linear_ramp(uint32_t size)
{
    GammaLUT lut(size);
    auto const step = std::numeric_limits<GammaLUT::value_type>::max() / (size - 1);
    GammaLUT::value_type n = 0;
    std::generate(lut.begin(), lut.end(), [&n, step]{ auto current = n; n += step; return current; });
    return lut;
}

class DrmVersion
{
public:
    DrmVersion(int fd) : ver(drmGetVersion(fd)) {}
    ~DrmVersion() { drmFreeVersion(ver); }

    drmVersionPtr operator->() { return ver; }
private:
    drmVersionPtr ver{nullptr};
};

class Drm
{
public:
    Drm() : fd(open("/dev/dri/card0", O_RDWR | O_CLOEXEC))
    {
        if (fd < 1)
            throw std::runtime_error("Could not open DRM device");
        if (!is_vc4())
            throw std::runtime_error("Not a VC4 drm device");
    }
    ~Drm() { close(fd); }

    bool is_vc4()
    {
        DrmVersion info(fd);
        return "vc4" == std::string(info->name);
    }

    void set_gamma()
    {
        uint32_t const size = 256;
        uint32_t const crtc_id = 48;

        GammaLUT lut = make_linear_ramp(size);
        if (drmModeCrtcSetGamma(fd, crtc_id, size, &lut[0], &lut[0], &lut[0]) != 0)
        {
            throw std::runtime_error("failed to set Gamma LUT on drm device");
        }
    }
private:
    int fd;
};
}

int main()
try
{
    Drm drm;
    drm.set_gamma();
}
catch (std::exception const& e)
{
    std::cerr << "error: " << e.what() << std::endl;
}
