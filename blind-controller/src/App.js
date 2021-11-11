import React, { useEffect } from 'react';
import './App.css';

import firebase from 'firebase';
import { firebaseConfig } from './config';

import Stack from '@mui/material/Stack';
import Chip from '@mui/material/Chip';
import TextField from '@mui/material/TextField';
import InputAdornment from '@mui/material/InputAdornment';
import Button from '@mui/material/Button';

export default function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h2>ESP32 Blind Controller</h2>
        <InputLux />
      </header>
    </div>
  );
}

const BLIND_NAME = "blind1";
firebase.initializeApp(firebaseConfig);
var firebaseTargetLux = firebase.database().ref(`${BLIND_NAME}/targetLux`);
var firebaseCurrentLux = firebase.database().ref(`${BLIND_NAME}/currentLux`);

function InputLux() {

  const [currentLux, setCurrentLux] = React.useState('');
  const [newLux, setNewLux] = React.useState('');

  useEffect(() => {
    firebaseCurrentLux.on('value', (dataSnapshot) => {
      setCurrentLux(dataSnapshot.val());
    });
  }, []);

  const handleClick = () => {
    const targetLux = parseInt(newLux, 10);
    if (!isNaN(targetLux)) {
      firebaseTargetLux.set(targetLux);
    }
  };

  return (
    <div>
      <Stack spacing={2} alignItems="center">
        <Stack direction="row" spacing={2} alignItems="center">
          <div>Current Lux:</div>
          <Chip label={`${currentLux} lx`} margin-left="5"/>
        </Stack>
        <Stack direction="row" spacing={2}>
          <TextField
            autoFocus
            label="Target Lux"
            onChange={(event) => {
              setNewLux(event.target.value);
            }}
            InputProps={{ endAdornment: <InputAdornment position="end">lx</InputAdornment> }}
          />
          <Button variant="contained" onClick={handleClick}>Set Lux</Button>
        </Stack>
      </Stack>
    </div>
  );
}