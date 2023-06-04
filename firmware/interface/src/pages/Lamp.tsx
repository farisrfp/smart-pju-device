import React from 'react';

import CircularSlider from '@fseehawer/react-circular-slider';

// import { SparklesIcon } from '@heroicons/react/24/outline';
import volIcon from '../asset/image 10.png';

const Lamp: React.FC = () => {
	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary text-center">
				<h1 className="mb-14 pt-10 text-2xl font-bold tracking-widest text-white">CONTROL</h1>
				<CircularSlider
					width={200}
					label="DIM"
					min={0}
					max={100}
					dataIndex={0}
					appendToValue="%"
					labelColor="#ffffff"
					knobColor="#3479ff"
					knobSize={50}
					progressColorFrom="#3479ff"
					progressColorTo="#25ced5"
					progressSize={15}
					trackColor="#eeeeee"
					trackSize={13}
				/>
				{/* <DragIcon x="22" y="22" width="28px" height="28px" /> */}
				{/* <SparklesIcon className="h-1 w-1 text-white" /> */}
				{/* </CircularSlider> */}

				<div className="mx-auto mt-10 h-20 w-5/6 rounded-lg bg-[#2F2F5C] px-5">
					<div className="flex h-full items-center justify-between">
						<div className="relative h-fit w-fit">
							<div className={`absolute -inset-0 z-10 rounded-sm bg-bluish blur-sm`} />
							<div className={`relative z-20 rounded-lg bg-bluish`}>
								<img src={volIcon} alt="XCamp" className="h-10 w-10 p-2" />
							</div>
						</div>
					</div>
				</div>
			</div>
		</>
	);
};

export default Lamp;
