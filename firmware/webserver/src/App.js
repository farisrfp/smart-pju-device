// import logo from './logo.svg';
import "./App.css";
import { w3cwebsocket as W3CWebSocket } from "websocket";
import React, { useState, useEffect, useRef } from "react";
// import Button from "@mui/material/Button";
// https://github.com/kokanek/web-socket-chat/blob/master/webSocketClient/src/index.js

function App() {
  const websocket = useRef(null);
  const [temp, setTemp] = useState(25);
  const [intensity, setIntensity] = useState(67);
  const [voltage, setVoltage] = useState(220);
  const [current, setCurrent] = useState(800);

  useEffect(() => {
    websocket.current = new W3CWebSocket("ws://192.168.2.1/ws");
    websocket.current.onmessage = message => {
      const dataFromServer = JSON.parse(message.data);
      console.log("got reply! ", dataFromServer);
      if (dataFromServer.type === "message") {
        setTemp(dataFromServer.temp);
        setIntensity(dataFromServer.intensity);
        setVoltage(dataFromServer.voltage);
        setCurrent(dataFromServer.current);
      }
    };
    return () => websocket.current.close();
  }, []);

  // function sendUpdate({ led = LED }) {
  //   websocket.current.send(
  //     JSON.stringify({
  //       type: "message",
  //       temp: temp,
  //       intensity: intensity,
  //       voltage: voltage,
  //       current: current
  //     })
  //   );
  // }

  // const LEDon = () => {
  //   sendUpdate({ led: true });
  // };
  // const LEDoff = () => {
  //   sendUpdate({ led: false });
  // };

  return (
    // <div className="body bg-sky-700">
    //   <div className="centered">
    //     <div className="wrapper">
    //       <h1>Currently {LED ? "ON" : "OFF"}</h1>
    //       <br />
    //       <Button variant="contained" onClick={LED ? LEDoff : LEDon}>
    //         {LED ? "Turn Off" : "Turn On"}
    //       </Button>
    //     </div>
    //   </div>
    // </div>
    <div className="bg-[#1D1E42] text-white h-screen ">
      <header className="bg-gradient-to-tr from-[#4071cc] to-[#164396] h-52 rounded-b-[50px] flex">
        <div className="p-7">
          <h1 className="text-xl font-semibold">Welcome</h1>
          <h2 className="mt-3 text-2xl font-bold">SMART PJU #12</h2>
        </div>
      </header>

      {/* RTC */}
      <div className="w-4/5 h-20 rounded-lg bg-[#141431] mx-auto -m-8">
        <div className="flex items-center h-full p-6 font-semibold text-lg">
          <div>
            <h3>Tuesday,</h3>
            <h3>19 October 2021</h3>
          </div>
          <div className="flex-grow">
            <h3 className="text-right">18:00</h3>
          </div>
        </div>
      </div>

      {/* Widget */}
      <div className="mt-16 grid grid-cols-2 gap-4 w-4/5 mx-auto">
        <div className="col-span-1 h-32 rounded-lg bg-[#1a479b]">
          <div className="flex flex-col h-full p-3 justify-between font-semibold text-lg">
            <h3>Temp</h3>
            <h3 className="self-end text-2xl">{temp}°C</h3>
          </div>
        </div>

        <div className="col-span-1 h-32 rounded-lg bg-[#1a479b]">
          <div className="flex flex-col h-full p-3 justify-between font-semibold text-lg">
            <h3>Intensity</h3>
            <h3 className="self-end text-2xl">{intensity} %</h3>
          </div>
        </div>

        <div className="col-span-1 h-32 rounded-lg bg-[#1a479b]">
          <div className="flex flex-col h-full p-3 justify-between font-semibold text-lg">
            <h3>Voltage</h3>
            <h3 className="self-end text-2xl">{voltage} V</h3>
          </div>
        </div>

        <div className="col-span-1 h-32 rounded-lg bg-[#1a479b]">
          <div className="flex flex-col h-full p-3 justify-between font-semibold text-lg">
            <h3>Current</h3>
            <h3 className="self-end text-2xl">{current} mA</h3>
          </div>
        </div>
      </div>
      {/*  */}

      {/*  */}
    </div>
  );
}

export default App;
