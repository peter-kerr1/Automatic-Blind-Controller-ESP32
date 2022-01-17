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

const BLIND_NAME = "blind1";

export default function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h2>ESP32 Blind Controller</h2>
        <Stack direction="row" spacing={5}>
          <Timers />
          <Buttons />
        </Stack>
      </header>
    </div>
  );
}

function Timers() {
  const [raiseTime, setRaiseTime] = React.useState(null);
  const [lowerTime, setLowerTime] = React.useState(null);

  const blindRaiseTime = firebase.database().ref(`${BLIND_NAME}/raiseTime`);
  const blindLowerTime = firebase.database().ref(`${BLIND_NAME}/lowerTime`);

  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/raiseTime`).on('value', (dataSnapshot) => {
      const time = new Date();
      time.setHours(Math.floor(dataSnapshot.val() / 100));
      time.setMinutes(dataSnapshot.val() % 100);
      setRaiseTime(time);
    });
    firebase.database().ref(`${BLIND_NAME}/lowerTime`).on('value', (dataSnapshot) => {
      const time = new Date();
      time.setHours(Math.floor(dataSnapshot.val() / 100));
      time.setMinutes(dataSnapshot.val() % 100);
      setLowerTime(time);
    });
  }, []);

  return (
    <LocalizationProvider dateAdapter={AdapterDateFns}>
      <Stack justifyContent="center" spacing={3}>
        <MobileTimePicker
          label="Raise Blind"
          value={raiseTime}
          onChange={(newValue) => {
            blindRaiseTime.set(newValue.getHours() * 100 + newValue.getMinutes());
          }}
          renderInput={(params) => <TextField {...params} />}
        />
        <MobileTimePicker
          label="Lower Blind"
          value={lowerTime}
          onChange={(newValue) => {
            blindLowerTime.set(newValue.getHours() * 100 + newValue.getMinutes());
          }}
          renderInput={(params) => <TextField {...params} />}
        />
      </Stack>
    </LocalizationProvider>
  );
}

function Buttons() {
  const blindCommand = firebase.database().ref(`${BLIND_NAME}/command`);
  const [state, setState] = React.useState('');

  // Whenever the command value changes on Firebase, update the buttons to reflect this change.
  // We wrap the event listener in a useEffect block with empty dependencies ([]) so that it is only run once (replicates the behaviour of componentDidMount().)
  useEffect(() => {
    firebase.database().ref(`${BLIND_NAME}/command`).on('value', (dataSnapshot) => {
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