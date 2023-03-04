namespace ParallelBuffer
{

    void run(std::vector<DataTuple> *data_tuples, int THREADS, int hashBits, const int PARTITIONS);
    void *buf_worker(void *arg);
}