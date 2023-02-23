namespace ParallelBuffer
{

    void run(std::vector<DataTuple> *data_tuples, int THREADS);
    void *buf_worker(void *arg);
}