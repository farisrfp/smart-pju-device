import React from 'react';
import { Link } from 'react-router-dom';

import { KeyboardArrowLeftOutlined } from '@mui/icons-material';

const LoRaWAN = () => {
	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary p-5 text-center">
				<h1 className="mb-8 pt-5 text-2xl font-bold tracking-widest text-white">SETTING</h1>

				<div className="h-[80%] w-full flex-col items-center rounded-md bg-tertiary px-5">
					<div className="relative mb-8 flex items-center pt-5">
						<Link to="/setting" className="">
							<KeyboardArrowLeftOutlined className=" text-bluish" />
						</Link>
						<h1 className="text-md absolute left-1/2 -translate-x-1/2 transform font-semibold tracking-widest text-white">
							LoRaWAN
						</h1>
					</div>
				</div>
			</div>
		</>
	);
};

export default LoRaWAN;
