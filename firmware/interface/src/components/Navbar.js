import React, { useState } from 'react';
import { Link } from 'react-router-dom';

import {
	ArrowUpTrayIcon,
	Cog6ToothIcon,
	LightBulbIcon,
	Squares2X2Icon,
} from '@heroicons/react/24/outline';

const Navbar = () => {
	const [active, setActive] = useState('home');

	return (
		<div className="fixed bottom-5 left-1/2 flex w-[350px] -translate-x-1/2 transform justify-around rounded-2xl bg-primary px-10 py-4">
			<Link to="/" onClick={() => setActive('home')}>
				<Squares2X2Icon className={`w-8 ${active === 'home' ? 'text-bluish' : 'text-white'}`} />
			</Link>
			<Link to="/pju" onClick={() => setActive('lamp')}>
				<LightBulbIcon className={`w-8 ${active === 'lamp' ? 'text-bluish' : 'text-white'}`} />
			</Link>
			<Link to="/upload" onClick={() => setActive('upload')}>
				<ArrowUpTrayIcon className={`w-8 ${active === 'upload' ? 'text-bluish' : 'text-white'}`} />
			</Link>
			<Link to="/setting" onClick={() => setActive('setting')}>
				<Cog6ToothIcon className={`w-8 ${active === 'setting' ? 'text-bluish' : 'text-white'}`} />
			</Link>
		</div>
	);
};

export default Navbar;
