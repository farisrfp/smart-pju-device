import React from 'react';

interface CardProps {
	type: string;
	value: number;
	icon: string;
}

const pickedType: any = {
	voltage: {
		color: 'bg-bluish',
		besaran: 'V',
		label: 'Voltage',
	},
	current: {
		color: 'bg-purplish',
		besaran: 'mA',
		label: 'Current',
	},
	temperature: {
		color: 'bg-pinkish',
		besaran: '°C',
		label: 'Temperature',
	},
	'light level': {
		color: 'bg-orangish',
		besaran: 'lux',
		label: 'Light Level',
	},
};

const Card: React.FC<CardProps> = ({ type, value, icon }) => {
	return (
		<div className="rounded-xl bg-primary">
			<div className="flex flex-wrap justify-between p-4">
				{/* Icon */}
				<div className="relative h-fit w-fit">
					<div className={`absolute -inset-0 z-10 rounded-sm blur-sm ${pickedType[type].color}`} />
					<div className={`relative z-20 rounded-lg ${pickedType[type].color}`}>
						<img src={icon} alt="XCamp" className="h-10 w-10 p-2" />
					</div>
				</div>
				{/* Value */}
				<div className="w-8/12 ">
					<p className="text-right text-2xl font-bold text-white">
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
