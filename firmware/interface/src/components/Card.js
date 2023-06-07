import React from 'react';

import { Bolt, DeviceThermostat, Flare, Power } from '@mui/icons-material';

const pickedType = {
	voltage: {
		color: 'bg-bluish',
		besaran: 'V',
		label: 'Voltage',
		icon: <Bolt className="text-white" />,
	},
	current: {
		color: 'bg-purplish',
		besaran: 'A',
		label: 'Current',
		icon: <Power className="text-white" />,
	},
	temperature: {
		color: 'bg-pinkish',
		besaran: '°C',
		label: 'Temperature',
		icon: <DeviceThermostat className="text-white" />,
	},
	'light level': {
		color: 'bg-orangish',
		besaran: 'lux',
		label: 'Light Level',
		icon: <Flare className="text-white" />,
	},
};

const Card = ({ type, value }) => {
	return (
		<div className="rounded-xl bg-primary">
			<div className="flex flex-wrap justify-between p-4">
				{/* Icon */}
				<div className="relative h-fit w-fit">
					<div
						className={`absolute -inset-0 z-10 rounded-lg blur-[3px] ${pickedType[type].color}`}
					/>
					<div
						className={`relative z-20 flex h-10 w-10 items-center justify-center rounded-lg ${pickedType[type].color}`}>
						{pickedType[type].icon}
					</div>
				</div>
				{/* Value */}
				<div className="w-8/12 ">
					<p className="text-right text-xl font-bold text-white">
						{value} {pickedType[type].besaran}
					</p>
				</div>
				{/* Label */}
				<div className="mt-4 text-sm font-bold tracking-wider text-white">
					{pickedType[type].label}
				</div>
			</div>
		</div>
	);
};

export default Card;
