namespace ParallelBuffer
{

    void run(std::vector<DataTuple> *data_tuples, int THREADS, int hashBits);
    void *buf_worker(void *arg);
}