void allocate_camera_memory()
{
    CLEAR(req);
    req.count = NUM_BUFFERS;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    xioctl(camera_fd, VIDIOC_REQBUFS, &req);

    printf("Video driver allocated %i buffers\n", req.count);
    
    buffers = calloc(req.count, sizeof(*buffers)); //changed

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
    {
        CLEAR(buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        xioctl(camera_fd, VIDIOC_QUERYBUF, &buf);

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = mmap(NULL, buf.length, //changed
                                              PROT_READ | PROT_WRITE, MAP_SHARED,
                                              camera_fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start) 
        {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }
}
