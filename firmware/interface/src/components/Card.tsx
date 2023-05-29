import React from 'react';

interface CardProps {
	type: string;
	value: number;
	icon: string;
}

const pickedType: any = {
	voltage: {
		color: 'bg-red-500',
		besaran: 'V',
		label: 'Voltage',
	},
	current: {
		color: 'bg-blue-500',
		besaran: 'mA',
		label: 'Current',
	},
	temperature: {
		color: 'bg-yellow-500',
		besaran: '°C',
		label: 'Temperature',
	},
	'light level': {
		color: 'bg-green-500',
		besaran: 'lux',
		label: 'Light Level',
	},
};

const Card: React.FC<CardProps> = ({ type, value, icon }) => {
	return (
		<div className="mx-auto -mt-20 w-5/12 rounded-xl bg-primary">
			<div className="flex flex-wrap justify-between p-4">
				{/* Icon */}
				<div className={`inline-block h-fit rounded-lg ${pickedType[type].color}`}>
					<img src={icon} alt="XCamp" className="h-10 p-2" />
				</div>
				{/* Value */}
				<div className="w-8/12 ">
					<p className="text-right text-2xl font-bold text-white">
						{value} {pickedType[type].besaran}
					</p>
				</div>
				{/* Label */}
				<div className="text-md mt-4 font-bold text-white">{pickedType[type].label}</div>
			</div>
		</div>
	);
};

export default Card;
