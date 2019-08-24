from time import time
import asyncio
from flexx import flx
from solar_serial import SolarSerial
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
        print(event.action)

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
        with flx.HSplit(flex=1):
            with flx.GroupWidget(flex=1, title="Plot options"):  
                    with flx.VBox():
                        with flx.HBox():
                            self.enable_load_button = flx.Button(text='Enable Load')
                            setattr(self.enable_load_button,'button_action', 'enable_load')
                            self.enable_load_button.reaction(self._on_button_pressed, 'pointer_click')
                            self.enable_load_radio = flx.RadioButton(text='')
                            flx.Widget(flex=1)

                        for i in range(0, self.plot_num):
                            self.plot_combobox[i] = flx.ComboBox(options=SolarData.params, editable=False, selected_index=i) 
                            self.plot_combobox[i].reaction(self.combobox_changed, 'user_selected')
                        flx.Widget(flex=1)

            with flx.VBox(flex=4):
                for i in range(0, self.plot_num):
                    self.plot[i] = flx.PlotWidget(flex=1, style='width: 640px; height: 320px;',xdata=[], yrange=(0, 100),ylabel='Plot ' + str(i+1))

    @flx.emitter
    def button_pressed(self, action):        
        return {'button_action': action}

    def _on_button_pressed(self, event):
        self.button_pressed(event.source.button_action)

    #clear data when plotting changed
    def combobox_changed(self, event):
        idx = self.plot_combobox.index(event.source)
        self.plot[idx].set_data([], [])

    @flx.action
    def update_info(self, info):
        # Set connections
        n = info.sessions, info.total_sessions
        self.status.set_html('There are %i connected clients.<br />' % n[0] +
                             'And in total we served %i connections.<br />' % n[1])

        if info.serial_data:
            data = info.serial_data
            for i in range(self.plot_num):
                param_idx = self.plot_combobox[i].selected_index
                param_name = SolarData.params[param_idx]
                
                times = list(self.plot[i].xdata)
                times.append(data['time'] - self.start_time)
                times = times[-self.nsamples:]

                ydata = list(self.plot[i].ydata)
                ydata.append(data[param_name])
                ydata = ydata[-self.nsamples:]

                self.plot[i].set_data(times, ydata)

# Create global relay
solarSerial = SolarSerial(SolarSettings.serial_settings)
solarSerial.start()
a = flx.App(Monitor)
a.serve()

def main_loop():
    if not solarSerial.recvQueue.empty():
        sd = solarSerial.recvQueue.get()
        relay.new_data(sd)
    asyncio.get_event_loop().call_later(1.0/SolarSettings.fps, main_loop)

main_loop()
flx.start()
solarSerial.stop()