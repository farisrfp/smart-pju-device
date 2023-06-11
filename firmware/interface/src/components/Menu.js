import React from 'react';

import {
	InfoOutlined,
	NetworkCheckOutlined,
	TuneOutlined,
	KeyboardArrowRightOutlined,
} from '@mui/icons-material';

const pickedType = {
	sensor: {
		label: 'Sensor',
		icon: <TuneOutlined className="text-bluish" />,
	},
	lorawan: {
		label: 'LoRaWAN',
		icon: <NetworkCheckOutlined className="text-bluish" />,
	},
	about: {
		label: 'About',
		icon: <InfoOutlined className="text-bluish" />,
	},
};

const Menu = ({ type }) => {
	return (
		<div className="flex h-14 w-full items-center rounded-md bg-tertiary px-5">
			{pickedType[type].icon}
			<p className="ml-3 text-left text-md font-semibold text-white">{pickedType[type].label}</p>
			<KeyboardArrowRightOutlined className="ml-auto text-bluish" />
		</div>
	);
};

export default Menu;
