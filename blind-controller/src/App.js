import React, { useEffect } from 'react';
import './App.css';

import firebase from 'firebase';
import { firebaseConfig } from './config';

import ToggleButton from '@material-ui/lab/ToggleButton';
import ToggleButtonGroup from '@material-ui/lab/ToggleButtonGroup';
import KeyboardArrowUpIcon from '@material-ui/icons/KeyboardArrowUp';
import StopIcon from '@material-ui/icons/Stop';
import KeyboardArrowDownIcon from '@material-ui/icons/KeyboardArrowDown';

export default function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h2>ESP32 Blind Controller</h2>
        <Buttons />
      </header>
    </div>
  );
}

firebase.initializeApp(firebaseConfig);
var blind_command = firebase.database().ref("command");

function Buttons() {
  const [state, setState] = React.useState('');

  // Whenever the command value changes on Firebase, update the buttons to reflect this change.
  // We wrap the event listener in a useEffect block with empty dependencies ([]) so that it is only run once (replicates the behaviour of componentDidMount().)
  useEffect(() => {
    blind_command.on('value', (dataSnapshot) => {
      setState(dataSnapshot.val());
    });
  }, []);

  // When one of the buttons is pressed, send the new value to Firebase
  const handleChange = (_event, newState) => {
    if (newState !== null) {
      blind_command.set(newState);
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