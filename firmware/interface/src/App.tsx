import React from 'react';
import { Routes, Route } from 'react-router-dom';

// import Login from './components/Login';

import Home from './pages/Home';
import Upload from './pages/Upload';
import Setting from './pages/Setting';
import Lamp from './pages/Lamp';
import Navbar from './components/Navbar';

import './App.css';

function App() {
	return (
		<>
			<div className="h-screen bg-gradient-to-br from-primary via-secondary to-primary">
				<Navbar />
				<Routes>
					<Route path="/" element={<Home />} />
					<Route path="/lamp" element={<Lamp />} />
					<Route path="/upload" element={<Upload />} />
					<Route path="/setting" element={<Setting />} />
				</Routes>
			</div>
		</>
	);
}

export default App;
