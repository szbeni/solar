from time import time
import asyncio
import signal
from flexx import flx
from solar_serial import SolarSerial
from solar_serial_publisher import SolarSerialPublisher
from solar_settings import SolarSettings
from solar_data import SolarData


class Relay(flx.Component):
    number_of_connections = flx.IntProp(settable=True)
    
    def init(self):
        self.update_number_of_connections()

    @flx.manager.reaction('connections_changed')
    def update_number_of_connections(self, *events):
        n = 0
        for name in flx.manager.get_app_names():
            sessions = flx.manager.get_connections(name)
            n += len(sessions)
        self.set_number_of_connections(n)

    @flx.emitter
    def new_data(self, serial_data):
        return dict(serial_data=serial_data,
                    sessions=self.number_of_connections,
                    total_sessions=flx.manager.total_sessions)

relay = Relay()

class Monitor(flx.PyWidget):
    def init(self):
        with flx.VBox():
            flx.Label(html='<h3>Server monitor</h3>')
            if flx.current_server().serving[0] == 'localhost':
                # Don't do this for a public server
                self.button = flx.Button(text='Do some work')
                self.button.reaction(self._do_work, 'pointer_down')
            self.view = MonitorView(flex=1)
            self.view.reaction(self.button_pressed, 'button_pressed')


    @relay.reaction('new_data')  # note that we connect to relay
    def _push_info(self, *events):
        if not self.session.status:
            return relay.disconnect('system_info:' + self.id)
        for ev in events:
            data = dict()
            data['serial_data'] = ev.serial_data.as_dict()
            data['sessions'] = ev.sessions
            data['total_sessions'] = ev.total_sessions
            self.view.update_info(data)

    def button_pressed(self, event):
        action = event['button_action']
        button_list = list(SolarData.commands.keys())
        if action in button_list:
            keystroke = SolarData.commands[action]['keystroke'].encode()
            solarSerial.sendQueue.put(keystroke)

    def _do_work(self, *events):
        etime = time() + len(events)
        print('doing work for %i s' % len(events))
        while time() < etime:
            pass

class MonitorView(flx.VBox):
    def init(self):
        self.nsamples = 200
        self.start_time = time()
        self.status = flx.Label(text='...')
        self.plot_num = 3
        self.plot = []
        self.plot_combobox = []
        self.plot_label = []
        self.plot_latest_val = []
        self.latest_params_names = {}
        self.latest_params_values = {}


        with flx.HSplit(flex=1):
            with flx.GroupWidget(flex=1, title="Plot options", minsize=(270,0)):  
                    with flx.VBox():
                        self.buttons = {}
                        button_list = list(SolarData.commands.keys())
                        for button_name in button_list:
                            with flx.HBox():    
                                self.buttons[button_name] = flx.Button(text=button_name)
                                setattr(self.buttons[button_name],'button_action', button_name)
                                self.buttons[button_name].reaction(self._on_button_pressed, 'pointer_click')
                                self.buttons[button_name].reaction(self._on_button_down, 'pointer_down')
                                self.buttons[button_name].reaction(self._on_button_up, 'pointer_up')
                                flx.Widget(flex=1)

                        for i in range(0, self.plot_num):
                            with flx.HBox():
                                self.plot_combobox[i] = flx.ComboBox(options=list(SolarData.params.keys()), editable=False, selected_index=i) 
                                self.plot_combobox[i].reaction(self.combobox_changed, 'user_selected')
                                self.plot_label[i] = flx.Label(text='Value:')
                                self.plot_latest_val[i] = flx.Label(text='')
                                flx.Widget(flex=1)

                        for i in range(0, len(SolarData.params.keys())):
                            with flx.HBox():
                                name = list(SolarData.params.keys())[i]
                                self.latest_params_names[name] = flx.Label(text='{0}:'.format(name))
                                self.latest_params_values[name] = flx.Label(text='NaN')
                                flx.Widget(flex=1)

                        flx.Widget(flex=1)

            with flx.VBox(flex=4):
                for i in range(0, self.plot_num):
                    self.plot[i] = flx.PlotWidget(flex=1, minsize=(270,0), style='width: 640px; height: 320px;',xdata=[], yrange=(0, 100),ylabel='Plot ' + str(i+1))
                    self.update_plot_range(i)


    @flx.emitter
    def button_pressed(self, action):        
        return {'button_action': action}

    @flx.emitter
    def button_down(self, action):        
        #print("button down")
        #return {'button_action': action}
        pass

    @flx.emitter
    def button_up(self, action):
        #print("button up")
        #return {'button_action': action}
        pass

    def _on_button_pressed(self, event):
        self.button_pressed(event.source.button_action)

    def _on_button_down(self, event):
        self.button_down(event.source.button_action)

    def _on_button_up(self, event):
        self.button_up(event.source.button_action)

    #clear data when plotting changed
    def combobox_changed(self, event):
        idx = self.plot_combobox.index(event.source)
        self.update_plot_range(idx)

    def update_plot_range(self,idx):
        self.plot[idx].set_data([], [])
        param = self.plot_combobox[idx].selected_key
        self.plot[idx].set_yrange(SolarData.params[param]['range'])

    @flx.action
    def update_info(self, info):
        # Set connections
        n = info.sessions, info.total_sessions
        self.status.set_html('There are %i connected clients.<br />' % n[0] +
                             'And in total we served %i connections.<br />' % n[1])

        if info.serial_data:
            data = info.serial_data
            for d in data:
                if d in self.latest_params_values:
                    self.latest_params_values[d].set_text(data[d])

            for i in range(self.plot_num):
                param_idx = self.plot_combobox[i].selected_index
                param_name = list(SolarData.params.keys())[param_idx]
                
                times = list(self.plot[i].xdata)
                times.append(data['time'] - self.start_time)
                times = times[-self.nsamples:]

                ydata = list(self.plot[i].ydata)
                ydata.append(data[param_name])
                ydata = ydata[-self.nsamples:]

                self.plot_latest_val[i].set_text(data[param_name])
                self.plot[i].set_data(times, ydata)

#solar serial
solarSerial = SolarSerial(SolarSettings.serial_settings)
solarSerial.start()

#serial publisher
solarSerialPublisher = SolarSerialPublisher(SolarSettings.serial_pub_settings)
solarSerialPublisher.start()

a = flx.App(Monitor)
a.serve()
flx.config.hostname = '0.0.0.0'
flx.config.port = 49190

def main_loop():
    if not solarSerial.recvQueue.empty():
        sd = solarSerial.recvQueue.get()
        relay.new_data(sd)
        solarSerialPublisher.new_data(str(sd).encode())
    asyncio.get_event_loop().call_later(1.0/SolarSettings.fps, main_loop)

main_loop()
flx.start()
solarSerial.stop()
solarSerialPublisher.stop()