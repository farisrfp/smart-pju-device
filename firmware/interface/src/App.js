import React from 'react';
import { Routes, Route } from 'react-router-dom';

import Login from './pages/Login';

import Home from './pages/Home';
import Upload from './pages/Upload';
import Setting from './pages/Setting';
import PJU from './pages/Pju';
import Navbar from './components/Navbar';
import Sensor from './pages/settings/Sensor';
import Lorawan from './pages/settings/Lorawan';
import About from './pages/settings/About';

import './App.css';

// import font
import './fonts/InterExtraBold.ttf';
import './fonts/AxiataBold.TTF';
import './fonts/AxiataMedium.TTF';
import './fonts/AxiataRegular.TTF';

function App() {
	return (
		<>
			{/* <div className="background-techdays relative h-screen font-axiata">
				<Login />
			</div> */}
			<div className="relative h-screen bg-gradient-to-br from-primary via-secondary to-primary font-axiata">
				<Navbar />
				<Routes>
					<Route path="/" element={<Home />} />
					<Route path="/pju" element={<PJU />} />
					<Route path="/upload" element={<Upload />} />
					<Route path="/setting" element={<Setting />} />
					<Route path="/setting/sensor" element={<Sensor />} />
					<Route path="/setting/lorawan" element={<Lorawan />} />
					<Route path="/setting/about" element={<About />} />
				</Routes>
			</div>
		</>
	);
}

export default App;
