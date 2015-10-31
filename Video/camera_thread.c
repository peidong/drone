void *start_camera() // time in seconds 0 means do forever
{
    struct queue_element *fresh_buf = NULL;
    struct v4l2_buffer local_buf;
        
    printf("Entering start_camera ...\n");

    for (i = 0; i < NUM_BUFFERS; ++i) 
    {
        CLEAR(local_buf);
        local_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        local_buf.memory = V4L2_MEMORY_MMAP;
        local_buf.index = i;
        xioctl(camera_fd, VIDIOC_QBUF, &local_buf);
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(camera_fd, VIDIOC_STREAMON, &type);

    do 
    {
        do 
        {
            FD_ZERO(&fds);
            FD_SET(camera_fd, &fds);

            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(camera_fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno = EINTR)));
        
        if (r == -1) 
        {
            perror("select");
            pthread_exit(NULL);
        }

        struct v4l2_buffer another_local_buf;
        
        CLEAR(another_local_buf);
        another_local_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        another_local_buf.memory = V4L2_MEMORY_MMAP;
        
        xioctl(camera_fd, VIDIOC_DQBUF, &another_local_buf);
        
        struct queue_element temp_q_element;
        
        temp_q_element.bytes_used = another_local_buf.bytesused;
        temp_q_element.index = another_local_buf.index;
        temp_q_element.timestamp.tv_sec = another_local_buf.timestamp.tv_sec;
        temp_q_element.timestamp.tv_usec = another_local_buf.timestamp.tv_usec;
        
        fresh_buf = used_q_add(&temp_q_element);
        
        if (NULL != fresh_buf)
        {
            CLEAR(another_local_buf);
            another_local_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            another_local_buf.memory = V4L2_MEMORY_MMAP;
            another_local_buf.index = fresh_buf->index;

            xioctl(camera_fd, VIDIOC_QBUF, &another_local_buf);
        }
        
    } while (1);
