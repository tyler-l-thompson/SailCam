from track_data import TrackParser
from resources import Tools


def main():
    parser = TrackParser(file_path='/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2020.csv')
    tools = Tools.Tools()
    tools.pretty_print_objects(objects=parser.track_data, objFilter='geo_d')
    print(f"Max Speed: {max(node.speed for node in parser.track_data)}kts")


if __name__ == '__main__':
    main()
