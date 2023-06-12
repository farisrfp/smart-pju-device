import React from 'react';

import { Bolt, DeviceThermostat, Flare, Power } from '@mui/icons-material';

const pickedType = {
	voltage: {
		color: 'bg-bluish',
		nilai: val => parseInt(val),
		besaran: 'V',
		label: 'Voltage',
		icon: <Bolt className="text-white" />,
	},
	current: {
		color: 'bg-purplish',
		nilai: val => parseFloat(val / 1000).toFixed(2),
		besaran: 'A',
		label: 'Current',
		icon: <Power className="text-white" />,
	},
	temperature: {
		color: 'bg-pinkish',
		nilai: val => parseInt(val),
		besaran: '°C',
		label: 'Temperature',
		icon: <DeviceThermostat className="text-white" />,
	},
	'light level': {
		color: 'bg-orangish',
		nilai: val => parseInt(val),
		besaran: 'lux',
		label: 'Light Level',
		icon: <Flare className="text-white" />,
	},
};

const Card = ({ type, value }) => {
	return (
		<div className="rounded-xl bg-primary">
			<div className="flex flex-col justify-around h-full px-4 py-2">
				{/* Label */}
				<div className={`w-full text-md font-bold tracking-wider text-white`}>
					{pickedType[type].label}
				</div>
				<div className="flex w-full items-center justify-between">
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
							{pickedType[type].nilai(value)} {pickedType[type].besaran}
						</p>
					</div>
				</div>
			</div>
		</div>
	);
};

export default Card;
