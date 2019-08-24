import time

class SolarData():
    params = [
        'Battery Voltage',
        'Battery Current',
        'Solar Voltage',
        'Solar Current',
        'Load Current',
        'Load Enable',
        'Solar Enable',
        'Solar Duty',
    ]

    time = 0

    def __init__(self, data_str=None):
        self.values = {}
        self.initalized = False

        for p in self.params:
            self.values[p] = None

        if data_str is not None:
            self.parse_data(data_str)

    def __str__(self):
        return "{}".format(self.values)

    def set_value(self,name, value):
        if name in self.params:
            self.values[name] = value

    def get_value(self, name):
        if name in self.params:
            return self.values[name]

    def from_dict(self, d):
        if 'time' in d:
            self.time = d['time']
            
        for k in self.values.keys():
            self.values[k] = d[k]
        

    def parse_data(self, data_str):
        data = data_str.strip().split(',')
        if len(data) == len(self.params):
            for i in range(0,len(data)):
                p = self.params[i]
                self.values[p] = data[i]                
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
    data.parse_data("1,2,3,4,5,6,7,8")
    print(data)
    data.set_value("Battery Voltage", 10)
    print(data)