import React from 'react';

import Login from './components/login';
import Card from './components/Card';

import volIcon from './asset/image 10.png';
import curIcon from './asset/image 9.png';
import tempIcon from './asset/image 7.png';
import lightIcon from './asset/image 8.png';

import {
	ArrowUpTrayIcon,
	Cog6ToothIcon,
	LightBulbIcon,
	Squares2X2Icon,
} from '@heroicons/react/24/outline';

function App() {
	return (
		<>
			<div className="flex h-screen flex-wrap items-center bg-slate-500">
				{/* <Login /> */}
				{/* <Card type="voltage" value={220} icon={volIcon} /> */}
				{/* <Card type="current" value={800} icon={curIcon} /> */}
				{/* <Card type="temperature" value={25} icon={tempIcon} /> */}
				{/* <Card type="light level" value={40} icon={lightIcon} /> */}
				{/* </div> */}
				{/* <div className="flex"> */}
				<div className="mx-5 flex w-full justify-between rounded-full bg-primary px-10 py-4">
					<Squares2X2Icon className="w-8 text-white" />
					<LightBulbIcon className="w-8 text-white" />
					<ArrowUpTrayIcon className="w-8 text-white" />
					<Cog6ToothIcon className="w-8 text-white" />
				</div>
			</div>
		</>
	);
}

export default App;
