from time import sleep
from threading import Thread
from abc import abstractmethod
from resources import Resource


class ThreadedClass(Thread, Resource):

    def __init__(self):
        Thread.__init__(self)
        Resource.__init__(self)
        self.runtime = 0

    @abstractmethod
    def run(self):
        self.logger.error(msg=self.__not_implemented_error__)
        raise NotImplementedError(self.__not_implemented_error__)

    @abstractmethod
    def join(self, **kwargs):
        self.logger.error(msg=self.__not_implemented_error__)
        raise NotImplementedError(self.__not_implemented_error__)


class ThreadPool(Resource):

    def __init__(self):
        Resource.__init__(self)

    def run_thread_pool(self, threads):
        done_threads = []
        thread_pool = []
        runtime_sum = 0
        threads_processed = 0
        total_thread_count = len(threads)

        # determine number of threads to use
        process_threads = self.config['process_threads']
        if len(threads) < process_threads:
            process_threads = len(threads)

        # fill thread pool
        for i in range(process_threads):
            thread_pool.append(threads.pop())

        self.logger.info(msg=f"Starting thread pool of size {self.config['process_threads']}")
        # start all threads in pool
        for thread in thread_pool:
            thread.start()

        self.logger.info(msg="Managing pool...")
        # manage the thread pool
        while threads:
            sleep(0.2)
            for thread in thread_pool:
                if not thread.is_alive():

                    # show runtime stats
                    if self.config['runtime_stats']:
                        runtime_sum += thread.runtime
                        threads_processed += 1
                        if threads_processed % self.config['runtime_stats'] == 0:
                            # calc average
                            average_runtime = runtime_sum / threads_processed

                            # calc time left
                            users_remaining = total_thread_count - threads_processed
                            seconds_left = users_remaining * average_runtime
                            minuets_left = int(seconds_left / 60)
                            units = 'minuets'
                            time_left = minuets_left
                            if minuets_left == 0:
                                time_left = int(seconds_left)
                                units = 'seconds'

                            percent = float(len(done_threads)) * 100 / total_thread_count
                            arrow = '-' * int(percent / 100 * self.config['progress_bar_length'] - 1) + '>'
                            spaces = ' ' * (self.config['progress_bar_length'] - len(arrow))
                            print('Progress: [%s%s] %d %%' % (arrow, spaces, percent), end=' ')

                            print('Average Runtime: {0:03d} ms,'
                                  ' ETA: {1:02d} {2}, Remaining: {3:05d}'.
                                  format(int(average_runtime * 1000), time_left, units, users_remaining), end='\r')

                    # shift threads in and out of thread pool
                    done_threads.append(thread)
                    thread_pool.remove(thread)
                    try:
                        new_thread = threads[0]
                        thread_pool.append(new_thread)
                        threads.pop(0)
                        new_thread.start()
                    except IndexError:
                        pass

        # wait for the last threads to finish
        last_threads_running = True
        while last_threads_running:
            sleep(0.2)
            last_threads_running = False
            for thread in thread_pool:
                if thread.is_alive():
                    last_threads_running = True
                else:
                    # show runtime stats
                    if self.config['runtime_stats']:
                        if thread.runtime != 0:
                            runtime_sum += thread.runtime
                            threads_processed += 1
                            thread.runtime = 0
                            if threads_processed % self.config['runtime_stats'] == 0:
                                # calc average
                                average_runtime = runtime_sum / threads_processed

                                # calc time left
                                users_remaining = total_thread_count - threads_processed
                                seconds_left = users_remaining * average_runtime
                                minuets_left = int(seconds_left / 60)
                                units = 'minuets'
                                time_left = minuets_left
                                if minuets_left == 0:
                                    time_left = int(seconds_left)
                                    units = 'seconds'

                                percent = float(len(done_threads)) * 100 / total_thread_count
                                arrow = '-' * int(percent / 100 * self.config['progress_bar_length'] - 1) + '>'
                                spaces = ' ' * (self.config['progress_bar_length'] - len(arrow))
                                print('Progress: [%s%s] %d %%' % (arrow, spaces, percent), end=' ')

                                print('Average Runtime: {0:03d} ms,'
                                      ' ETA: {1:02d} {2}, Remaining: {3:05d}'.
                                      format(int(average_runtime * 1000), time_left, units, users_remaining), end='\r')
        if self.config['runtime_stats']:
            print()
        self.logger.info(msg="Joining threads...")
        # join all threads
        for thread in done_threads:
            thread.join()
        self.logger.info(msg="Done.")
