namespace ParallelBuffer
{

    struct Payload
    {
        size_t start_idx;
        size_t end_idx;
        std::vector<DataTuple> *buffer;
        std::vector<DataTuple> *chunks;
    };
    void run(std::vector<DataTuple> *data_tuples, int THREADS);
    void *buf_worker(void *arg);
}