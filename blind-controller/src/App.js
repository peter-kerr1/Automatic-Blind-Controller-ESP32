import React from 'react';
import './App.css';

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

function Buttons() {
  const [state, setState] = React.useState('stop');

  const handleChange = (event, newState) => {
    if (newState !== null) {
      setState(newState);
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