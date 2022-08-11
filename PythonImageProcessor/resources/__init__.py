from abc import ABCMeta
from resources import logging, config


class Resource:
    __metaclass__ = ABCMeta
    __not_implemented_error__ = "function has not been implemented in the subclass."

    def __init__(self):
        self.logger = logging.get_logger(__name__)
        self.config = config.config
