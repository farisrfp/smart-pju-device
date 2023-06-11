import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { Link } from 'react-router-dom';

import { KeyboardArrowLeftOutlined } from '@mui/icons-material';

import Button from '../../components/Button';

const saveInfo = (voltage, current) => {
	// Send a POST request
	axios({
		method: 'post',
		url: '/setting/sensor',
		data: 'volt_sens=' + voltage + '&curr_sens=' + current,
		headers: {
			'Content-Type': 'application/x-www-form-urlencoded',
		},
	})
		.then(res => {
			console.log(res.data);
		})
		.catch(err => {
			console.log(err);
		});
};

const Sensor = () => {
	const [voltage, setVoltage] = useState(1000);
	const [current, setCurrent] = useState(2200);

	useEffect(() => {
		// get data from api
		axios
			.get('/setting/sensor')
			.then(res => {
				const { volt_sens, curr_sens } = res.data;
				// string float to int
				setVoltage(volt_sens);
				setCurrent(curr_sens);
			})
			.catch(err => {
				console.log(err);
			});
	}, []);

	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary p-5 text-center">
				<h1 className="mb-8 pt-5 text-2xl font-bold tracking-widest text-white">SETTING</h1>

				<div className="w-full flex-col items-center rounded-md bg-tertiary px-5">
					<div className="relative mb-5 flex items-center pt-5">
						<Link to="/setting" className="">
							<KeyboardArrowLeftOutlined className=" text-bluish" />
						</Link>
						<h1 className="text-md absolute left-1/2 -translate-x-1/2 transform font-semibold tracking-widest text-white">
							Sensor
						</h1>
					</div>
					{/* Voltage */}
					<div className="mb-5 flex h-14 items-center rounded-lg bg-primary px-3 text-sm">
						<p className="w-1/2 text-white">Voltage Sensitivity</p>
						{/* input */}
						<input
							type="number"
							className="h-8 w-1/2 rounded-lg bg-tertiary text-center text-white"
							value={voltage}
							// set as integer
							onChange={e => setVoltage(parseInt(e.target.value))}
						/>
					</div>
					{/* Current */}
					<div className="flex h-14 items-center rounded-lg bg-primary px-3 text-sm">
						<p className="w-1/2 text-white">Current Sensitivity</p>
						{/* input */}
						<input
							type="number"
							value={current}
							className="h-8 w-1/2 rounded-lg bg-tertiary text-center text-white"
							onChange={e => setCurrent(parseInt(e.target.value))}
						/>
					</div>
					<div className="pb-5" />
				</div>
				<Button className={'mt-5 bg-tertiary px-8'} onClick={() => saveInfo(voltage, current)}>
					SAVE
				</Button>
			</div>
		</>
	);
};

export default Sensor;
