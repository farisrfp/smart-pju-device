import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { Link } from 'react-router-dom';

import { KeyboardArrowLeftOutlined } from '@mui/icons-material';

import Form from '../../components/Form';
import Button from '../../components/Button';

const saveInfo = (appEUI, devEUI, appKey) => {
	// Send a POST request
	axios({
		method: 'post',
		url: '/setting/lorawan',
		data: 'app_eui=' + appEUI + '&dev_eui=' + devEUI + '&app_key=' + appKey,
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

const LoRaWAN = () => {
	const [appEUI, setAppEUI] = useState('ffffffffffffffff');
	const [devEUI, setDevEUI] = useState('c3e005d07ed5b370');
	const [appKey, setAppKey] = useState('44f393f26fa81504493d6699523e2251');

	useEffect(() => {
		axios
			.get('/setting/lorawan')
			.then(res => {
				const { dev_eui, app_eui, app_key } = res.data;
				setDevEUI(dev_eui);
				setAppEUI(app_eui);
				setAppKey(app_key);
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
							LoRaWAN
						</h1>
					</div>

					{/* APP EUI */}
					<Form
						label="AppEUI"
						// UpperCase
						value={appEUI.toUpperCase()}
						variant={1}
						onChange={e => setAppEUI(e.target.value)}
					/>
					{/* DEV EUI */}
					<Form
						label="DevEUI"
						value={devEUI.toUpperCase()}
						variant={1}
						onChange={e => setDevEUI(e.target.value)}
					/>
					{/* APP KEY */}
					<Form
						label="AppKey"
						value={appKey.toUpperCase()}
						variant={1}
						onChange={e => setAppKey(e.target.value)}
					/>

					<div className="pb-3" />
				</div>

				<Button
					className={'mt-5 bg-tertiary px-8'}
					onClick={() => saveInfo(appEUI, devEUI, appKey)}>
					SAVE
				</Button>
			</div>
		</>
	);
};

export default LoRaWAN;
