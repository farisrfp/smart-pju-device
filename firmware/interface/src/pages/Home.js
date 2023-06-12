import React, { useState, useEffect } from 'react';
// import { w3cwebsocket as W3CWebSocket } from 'websocket';
import axios from 'axios';

import Card from '../components/Card';
import DateTime from '../components/DateTime';
import GlowingText from '../components/utils/GlowingText';

import xcamp from './xcamp2.png';

const Home = () => {
	const [vol, setVol] = useState(221);
	const [cur, setCur] = useState(800);
	const [temp, setTemp] = useState(25);
	const [light, setLight] = useState(40);
	const [rtc, setRtc] = useState(1685435353);

	const CardItem = [
		{
			type: 'voltage',
			value: vol,
		},
		{
			type: 'current',
			value: cur,
		},
		{
			type: 'temperature',
			value: temp,
		},
		{
			type: 'light level',
			value: light,
		},
	];

	useEffect(() => {
		// get data from api
		axios
			.get('/sensor')
			.then(res => {
				const { voltage, current, temperature, light, rtc } = res.data;
				// string float to int
				setVol(voltage);
				setCur(current);
				setTemp(temperature);
				setLight(light);
				setRtc(rtc);
			})
			.catch(err => {
				console.log(err);
			});
	}, []);

	return (
		<>
			<div className="py-5" />
			<div className="relative mx-auto h-[75%] w-[350px] pt-10">
				<img src={xcamp} alt="xcamp" className="absolute right-3 top-0 w-36" />
				{/* <img src={'./xcamp2.png'} alt="xcamp" className="absolute right-3 top-5 w-32" /> */}
				{/* WELCOME */}
				<div className="flex h-[25%] flex-col justify-evenly ">
					<GlowingText
						blur="blur-[2px]"
						className="gradient-purple bg-clip-text text-4xl font-extrabold text-transparent">
						WELCOME!
					</GlowingText>
					<GlowingText
						blur="blur-[2px]"
						className="ml-8 text-4xl font-extrabold tracking-widest text-white">
						X-LIGHT PJU
					</GlowingText>
				</div>

				{/* GRID */}
				<div className="grid h-[75%] grid-cols-2 gap-y-3 gap-x-2">
					<DateTime unix={rtc} />

					{CardItem.map((item, index) => (
						<Card key={index} type={item.type} value={item.value} />
					))}
				</div>
			</div>
		</>
	);
};

export default Home;
