import React from 'react';
import { Routes, Route } from 'react-router-dom';

// import Login from './components/Login';

import Home from './pages/Home';
import Upload from './pages/Upload';
import Setting from './pages/Setting';
import PJU from './pages/Pju';
import Navbar from './components/Navbar';

import './App.css';

// import font
import './fonts/AxiataBold.TTF';
import './fonts/AxiataMedium.TTF';
import './fonts/AxiataRegular.TTF';

function App() {
	return (
		<>
			<div className="h-screen bg-gradient-to-br from-primary via-secondary to-primary font-axiata">
				<Navbar />
				<Routes>
					<Route path="/" element={<Home />} />
					<Route path="/pju" element={<PJU />} />
					<Route path="/upload" element={<Upload />} />
					<Route path="/setting" element={<Setting />} />
				</Routes>
			</div>
		</>
	);
}

export default App;
