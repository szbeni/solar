import time
import re

class SolarData():
    commands = {
        'Toggle DCDC': {'keystroke': '1'},
        'DCDC Duty Decrease': {'keystroke': '2'},
        'DCDC Duty Increase': {'keystroke': '3'},
        'Toggle Load': {'keystroke': '4'},
        'Toggle Fan': {'keystroke': '5'},
        'Toggle MPPT': {'keystroke': '6'},
        
    }
    params = {
        'Battery Voltage': {'abbrev': 'BV', 'range': (10, 15) },
        'Battery Current': {'abbrev': 'BC', 'range': (-10, 10)},
        'Solar Voltage': {'abbrev': 'SV', 'range': (0, 40)},
        'Solar Current': {'abbrev': 'SC', 'range': (-10, 10)},
        'Load Current': {'abbrev': 'LC', 'range': (-10, 10)},
        'DCDC Enable': {'abbrev': 'DE', 'range': (0, 1)},
        'DCDC Duty': {'abbrev': 'DD', 'range': (0, 100)},
        'MPPT Enable': {'abbrev': 'ME', 'range': (0, 1)},
        'MPPT Deadtime': {'abbrev': 'MD', 'range': (0, 1000)},
        'MPPT Voltage': {'abbrev': 'MV', 'range': (0, 40)},
        'MPPT Direction': {'abbrev': 'MR', 'range': (0, 1)},
        'MPPT Power': {'abbrev': 'MP', 'range': (0, 200)},
        'MPPT State': {'abbrev': 'MS', 'range': (0, 2)},
        'Load Enable': {'abbrev': 'LE', 'range': (0, 1)},
        'Fan Speed': {'abbrev': 'FS', 'range': (0, 255)},
        'Error Counter': {'abbrev': 'EC', 'range': (0, 255)}
    }
    
    time = 0

    def __init__(self, data_str=None):
        self.values = {}
        self.initalized = False

        for p in self.params.keys():
            self.values[p] = None

        if data_str is not None:
            self.parse_data(data_str)

    def __str__(self):
        return "{}".format(self.values)

    def set_value(self,name, value):
        if name in self.params.keys():
            self.values[name] = value

    def get_value(self, name):
        if name in self.params.keys():
            return self.values[name]

    def from_dict(self, d):
        if 'time' in d:
            self.time = d['time']
            
        for k in self.values.keys():
            self.values[k] = d[k]
        

    def parse_data(self, data_str):
        data = re.findall(r"([a-zA-Z]+)([0-9\-\.]+)", data_str)
        if len(data) == len(self.params):
            for i in range(0,len(data)):
                for p in self.params:
                    abbrev = self.params[p]['abbrev']
                    if (abbrev == data[i][0]):
                        v = data[i][1]
                        self.values[p] = float(v) if '.' in v else int(v)
                        break
            self.initalized = True
            self.time = time.time()
            return True
        return False

    def as_dict(self):
        retval = self.values.copy()
        retval['time'] = self.time
        return retval

if __name__ == "__main__":
    data = SolarData()
    print(data)
    retval = data.parse_data('SV12.356BV12.668SC-0.001BC0.759LC0.791DD0DE0MD0MV0.000MR0MP0.000ME0LE1FE0')
    print(retval)
    print(data)
    data.set_value("Battery Voltage", 10)
    print(data)