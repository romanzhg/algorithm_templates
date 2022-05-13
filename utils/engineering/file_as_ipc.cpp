// 1. Create file
int robot_status_fd = open(mob_pnc::kRobotStatusFilename,
                           O_WRONLY | O_CREAT | O_TRUNC,
                           S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

if (robot_status_fd == -1) {
ROS_FATAL("Failed to open robot status file.");
exit(0);
}

// 2. Write a fixed length data.
// Note, this function is not used for now.
inline void RefreshRobotStatusFile(int fd, const char* data, int data_len) {
    int r0 = flock(fd, LOCK_EX);
    if (r0 == -1) {
        exit(0);
    }

    off_t r1 = lseek(fd, 0, SEEK_SET);
    if (r1 == -1) {
        exit(0);
    }
    ssize_t r2 = write(fd, data, data_len);
    if (r2 == -1) {
        exit(0);
    } else if (r2 != data_len) {
        exit(0);
    }

    int r3 = ftruncate(fd, data_len);
    if (r3 == -1) {
        exit(0);
    }

    r0 = flock(fd, LOCK_UN);
    if (r0 == -1) {
        exit(0);
    }
}
