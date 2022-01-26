import React, { useEffect } from 'react';
import './App.css';

import firebase from 'firebase';

import Stack from '@mui/material/Stack';
import TextField from '@mui/material/TextField';
import LocalizationProvider from '@mui/lab/LocalizationProvider';
import AdapterDateFns from '@mui/lab/AdapterDateFns';
import MobileTimePicker from '@mui/lab/MobileTimePicker';

import ToggleButton from '@material-ui/lab/ToggleButton';
import ToggleButtonGroup from '@material-ui/lab/ToggleButtonGroup';
import KeyboardArrowUpIcon from '@material-ui/icons/KeyboardArrowUp';
import StopIcon from '@material-ui/icons/Stop';
import KeyboardArrowDownIcon from '@material-ui/icons/KeyboardArrowDown';

import Slider from '@mui/material/Slider';

import Snackbar from '@mui/material/Snackbar';
import Alert from '@mui/material/Alert';

const BLIND_NAME = "blind1";

export default function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h2 style={{marginBottom: "45px"}}>ESP32 Blind Controller</h2>
        <Stack
          sx={{ height: 200, width: 350 }}
          justifyContent="space-between"
          alignItems="center"
          direction="row"
          spacing={4}
        >
          <BlindPos />
          <Timers />
          <Buttons />
        </Stack>
        <Lux />
      </header>
    </div>
  );
}

function BlindPos() {
  const [encoderMin, setEncoderMin] = React.useState(0);
  const [encoderMax, setEncoderMax] = React.useState(1);
  const [encoderVal, setEncoderVal] = React.useState(0);

  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/encoderMin`).on('value', (dataSnapshot) => {
      setEncoderMin(dataSnapshot.val());
    });
    firebase.database().ref(`${BLIND_NAME}/encoderMax`).on('value', (dataSnapshot) => {
      setEncoderMax(dataSnapshot.val());
    });
    firebase.database().ref(`${BLIND_NAME}/encoderVal`).on('value', (dataSnapshot) => {
      setEncoderVal(dataSnapshot.val());
    });
  }, []);

  const calculatePercent = (value) => {
    return `${(((value - encoderMin) / (encoderMax - encoderMin)) * 100).toFixed(1)}%`;
  };

  return (
    <Slider
      orientation="vertical"
      track="inverted"
      color='secondary'
      min={encoderMin}
      max={encoderMax}
      value={encoderVal}
      valueLabelFormat={calculatePercent}
      valueLabelDisplay="on"
    />
  );
}


function Timers() {
  const [raiseTime, setRaiseTime] = React.useState(null);
  const [lowerTime, setLowerTime] = React.useState(null);
  const [notification, setNotification] = React.useState(false);
  const [notifMessage, setNotifMessage] = React.useState("");

  const blindRaiseTime = firebase.database().ref(`${BLIND_NAME}/settings/raiseTime`);
  const blindLowerTime = firebase.database().ref(`${BLIND_NAME}/settings/lowerTime`);

  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/settings/raiseTime`).on('value', (dataSnapshot) => {
      const time = new Date();
      time.setHours(Math.floor(dataSnapshot.val() / 100));
      time.setMinutes(dataSnapshot.val() % 100);
      setRaiseTime(time);
      setNotification(true);
      setNotifMessage(`Raise time set to ${time.toLocaleTimeString('en-US', {hour: 'numeric', minute: 'numeric', hour12: true})}`)
    });
    firebase.database().ref(`${BLIND_NAME}/settings/lowerTime`).on('value', (dataSnapshot) => {
      const time = new Date();
      time.setHours(Math.floor(dataSnapshot.val() / 100));
      time.setMinutes(dataSnapshot.val() % 100);
      setLowerTime(time);
      setNotification(true);
      setNotifMessage(`Lower time set to ${time.toLocaleTimeString('en-US', {hour: 'numeric', minute: 'numeric', hour12: true})}`)
    });
  }, []);

  const closeNotification = (_, reason) => {
    if (reason === 'clickaway') {
      return;
    }
    setNotification(false)
  };

  return (
    <LocalizationProvider dateAdapter={AdapterDateFns}>
      <Stack justifyContent="center" spacing={3}>
        <MobileTimePicker
          label="Raise Time"
          value={raiseTime}
          onChange={(newValue) => {
            blindRaiseTime.set(newValue.getHours() * 100 + newValue.getMinutes());
          }}
          renderInput={(params) => <TextField {...params} />}
        />
        <MobileTimePicker
          label="Lower Time"
          value={lowerTime}
          onChange={(newValue) => {
            blindLowerTime.set(newValue.getHours() * 100 + newValue.getMinutes());
          }}
          renderInput={(params) => <TextField {...params} />}
        />
      </Stack>
      <Snackbar
        style={{marginLeft: 0, marginBottom: "20px"}}
        anchorOrigin={{vertical:'bottom', horizontal:'center'}}
        open={notification}
        onClose={closeNotification}
        autoHideDuration={4000}
      >
        <Alert severity="success">
          {notifMessage}
        </Alert>
      </Snackbar>
    </LocalizationProvider>
  );
}


function Buttons() {
  const [state, setState] = React.useState('');
  
  const blindCommand = firebase.database().ref(`${BLIND_NAME}/settings/command`);
  
  // Whenever the command value changes on Firebase, update the buttons to reflect this change.
  // We wrap the event listener in a useEffect block with empty dependencies ([]) so that it is only run once (replicates the behaviour of componentDidMount().)
  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/settings/command`).on('value', (dataSnapshot) => {
      setState(dataSnapshot.val());
    });
  }, []);

  // When one of the buttons is pressed, send the new value to Firebase
  const handleChange = (_event, newState) => {
    if (newState !== null) {
      blindCommand.set(newState);
    }
  };

  return (
    <ToggleButtonGroup orientation="vertical" size="large" value={state} onChange={handleChange} exclusive>
      <ToggleButton value="up">
        <KeyboardArrowUpIcon/>
      </ToggleButton>
      <ToggleButton value="stop">
        <StopIcon/>
      </ToggleButton>
      <ToggleButton value="down">
        <KeyboardArrowDownIcon/>
      </ToggleButton>
    </ToggleButtonGroup>
  );
}


function Lux() {
  const [currentLux, setCurrentLux] = React.useState(0);

  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/currentLux`).on('value', (dataSnapshot) => {
      setCurrentLux(dataSnapshot.val());
    });
  }, []);

  return (
    <div style={{marginTop:"10px", marginBottom:"150px"}}>
      Current lux: {currentLux}lx
    </div>
  );
}