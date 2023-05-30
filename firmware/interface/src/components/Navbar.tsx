import React from 'react';
import { Link } from 'react-router-dom';

import {
	ArrowUpTrayIcon,
	Cog6ToothIcon,
	LightBulbIcon,
	Squares2X2Icon,
} from '@heroicons/react/24/outline';

const Navbar: React.FC = () => {
	return (
		<div className="fixed bottom-5 left-1/2 flex w-[350px] -translate-x-1/2 transform justify-around rounded-2xl bg-primary px-10 py-4">
			<Link to="/">
				<Squares2X2Icon className="w-8 text-white" />
			</Link>
			<Link to="/lamp">
				<LightBulbIcon className="w-8 text-white" />
			</Link>
			<Link to="/upload">
				<ArrowUpTrayIcon className="w-8 text-white" />
			</Link>
			<Link to="/setting">
				<Cog6ToothIcon className="w-8 text-white" />
			</Link>
		</div>
	);
};

export default Navbar;
