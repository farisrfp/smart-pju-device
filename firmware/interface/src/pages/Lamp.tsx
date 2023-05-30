import React from 'react';

import CircularSlider from '@fseehawer/react-circular-slider';

// import { SparklesIcon } from '@heroicons/react/24/outline';

const Lamp: React.FC = () => {
	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary text-center">
				<h1 className="pb-15 pt-10 text-2xl font-bold text-white">CONTROL</h1>
				<CircularSlider
					width={180}
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
			</div>
		</>
	);
};

export default Lamp;
