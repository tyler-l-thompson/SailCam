import optparse


def main():
    args = get_args()


def get_args():
    """
    Get command line arguments.
    :return: optparse.options
    """
    parser = optparse.OptionParser(usage='%prog [options]',
                                   description=""
                                   )
    parser.add_option("-v",
                      "--verbose",
                      default=False,
                      action="store_true"
                      )

    options, args = parser.parse_args()
    return options


if __name__ == '__main__':
    main()
