import threading
from abc import abstractmethod
from PythonImageProcessor.resources import Resource


class ThreadedClass(Resource, threading.Thread):

    def __init__(self):
        super().__init__()

    @abstractmethod
    def run(self):
        self.logger.error(msg=self.__not_implemented_error__)
        raise NotImplementedError(self.__not_implemented_error__)

    @abstractmethod
    def join(self, **kwargs):
        self.logger.error(msg=self.__not_implemented_error__)
        raise NotImplementedError(self.__not_implemented_error__)
