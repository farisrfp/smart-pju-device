import React, { useState, useEffect } from 'react';

const Days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
const Months = [
	'January',
	'Febuary',
	'March',
	'April',
	'May',
	'June',
	'July',
	'August',
	'Septemper',
	'October',
	'November',
	'December',
];

const DateTime = ({ unix }) => {
	const [date, setDate] = useState(new Date(unix * 1000));

	useEffect(() => {
		setDate(new Date(unix * 1000));
	}, [unix]);

	return (
		<div className="relative col-span-2">
			<div className="absolute inset-0 rounded-xl bg-gradient-to-r from-bluish to-purplish blur-sm" />
			<div className="relative z-10 flex h-full items-center justify-between rounded-xl bg-gradient-to-r from-bluish to-purplish p-5">
				<div className="font-inter text-2xl font-extrabold text-primary">
					<h2 className="mb-2 tracking-wider">{Days[date.getDay()]}</h2>
					{/* Format DD:MONTH:2022 */}
					<h2>{date.getDate() + ' ' + Months[date.getMonth()] + ' ' + date.getFullYear()}</h2>
				</div>
				<h2 className="text-4xl font-extrabold text-primary">
					{date.toLocaleTimeString('en-US', {
						hour: '2-digit',
						minute: '2-digit',
						hour12: false,
					})}
				</h2>
			</div>
		</div>
	);
};

export default DateTime;
