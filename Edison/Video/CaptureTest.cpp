#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <libv4l2.h>

long diff(struct timespec start, struct timespec end)
{
    struct timespec     temp;

    if ((end.tv_nsec-start.tv_nsec) < 0) 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = (temp.tv_sec *1000000000) + end.tv_nsec - start.tv_nsec;
    } 
    else 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = (temp.tv_sec *1000000000) + end.tv_nsec - start.tv_nsec;
    }
    
   return (long) temp.tv_nsec;
}


int xioctl(int fd, int request, void * arg)
{
	int r;

	do
	{
		r = ioctl(fd, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}

int print_caps(int fd)
{
	struct v4l2_capability caps = {};
	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps))
	{
		perror("Querying Capabilities");
		return 1;
	}

	printf( "Driver Caps:\n"
			"  Driver: \"%s\"\n"
			"  Card: \"%s\"\n"
			"  Bus: \"%s\"\n"
			"  Version: %d.%d\n"
			"  Capabilities: %08x\n",
			caps.driver,
			caps.card,
			caps.bus_info,
			(caps.version >> 16) && 0xff,
			(caps.version >> 24) && 0xff,
			caps.capabilities);

	struct v4l2_cropcap cropcap = {};
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
	{
		perror("Querying Cropping Capabilities");
		return 1;
	}

	printf( "Camera Cropping:\n"
			"  Bounds: %dx%d+%d+%d\n"
			"  Default: %dx%d+%d+%d\n"
			"  Aspect: %d/%d\n",
			cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, cropcap.bounds.top,
			cropcap.defrect.width, cropcap.defrect.height, cropcap.defrect.left, cropcap.defrect.top,
			cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);

	struct v4l2_fmtdesc fmtdesc = {};
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	char fourcc[5] = {};
	char c, e;
	printf("  FMT : CE Desc\n--------------------\n");
	while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
	{
		strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
		c = (fmtdesc.flags & 1) ? 'C' : ' ';
		e = (fmtdesc.flags & 2) ? 'E' : ' ';
		printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
		fmtdesc.index++;
	}

	return 0;
}

int set_image_format(int fd)
{
    v4l2_streamparm *setfps;
    
	struct v4l2_format fmt = {};
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = 320;
	fmt.fmt.pix.height = 240;
//        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
	{
		perror("Setting Pixel Format");
		return 1;
	}

	char fourcc[5] = {};
	strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
	printf( "Selected Camera Mode:\n"
			"  Width: %d\n"
			"  Height: %d\n"
			"  PixFmt: %s\n"
			"  Field: %d\n",
			fmt.fmt.pix.width,
			fmt.fmt.pix.height,
			fourcc,
			fmt.fmt.pix.field);
	return 0;
}

int init_mmap(int fd)
{
    struct v4l2_requestbuffers req = {};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
    {
        perror("Requesting Buffer");
        return 1;
    }

    struct v4l2_buffer buf = {};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
    {
        perror("Querying Buffer");
        return 1;
    }

    uint8_t * buffer;
    buffer = (uint8_t *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);

    return 0;
}

int capture_image(int fd)
{
    long delta;
    struct timespec time1;
    struct timespec time2;
    struct timespec time3;
    struct timespec time4;
    struct timespec time5;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

    struct v4l2_buffer buf = {};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    {
        perror("Query Buffer");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    delta = diff(time1, time2);
    printf("Profile (QBUF): %lu\n", delta);

    // Modified logic to start capture only once to avoid kernel panic.
    static fd_set fds;
    static bool firstRun = true;
    if (firstRun)
    {
    	firstRun = false;

		if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type))
		{
			perror("Start Capture");
			return 1;
		}

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

                clock_gettime(CLOCK_MONOTONIC_RAW, &time3);
                delta = diff(time2, time3);
                printf("Profile (STREAMON): %lu\n", delta);
    }

    struct timeval tv = {};
    tv.tv_sec = 2;
    int r = select(fd + 1, &fds, NULL, NULL, &tv);
    if (-1 == r)
    {
        perror("Waiting for Frame");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time4);

    delta = diff(time3, time4);
    printf("Profile (SELECT): %lu\n", delta);

    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
        perror("Retrieving Frame");
        return 1;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &time5);
    delta = diff(time4, time5);
    printf("Profile (DQBUF): %lu\n", delta);

    return 0;
}

int main()
{
	int fd;

	fd = open("/dev/video0", O_RDWR | O_NONBLOCK);
	if (fd == -1)
	{
		perror("Opening Video Device");
		return 1;
	}

	if (print_caps(fd))
	{
		close(fd);
		return 1;
	}

	if (set_image_format(fd))
	{
		close(fd);
		return 1;
	}

	if (init_mmap(fd))
	{
		close(fd);
		return 1;
	}

	for (int i = 0; i < 10; ++i)
	{
            if (capture_image(fd))
            {
                    close(fd);
                    return 1;
            }
	}

	close(fd);
	return 0;
}
