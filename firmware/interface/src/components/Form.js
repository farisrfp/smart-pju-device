import React from 'react';

const classCustom = [
	{
		div: 'py-2 items-center',
		p: 'w-1/2',
		input: 'w-1/2 h-8',
	},
	{
		div: 'flex-col',
		p: 'text-left h-6 mt-1',
		input: 'w-full mb-3 h-6',
	},
];

const Form = ({ label, variant, onChange, value }) => {
	return (
		<div
			className={`mb-3 flex h-fit rounded-lg bg-primary px-3 text-sm ${classCustom[variant].div}`}>
			<p className={` text-white ${classCustom[variant].p}`}>{label}</p>
			{/* input */}
			<input
				type="text"
				className={`rounded-md bg-tertiary px-2 text-center text-white ${classCustom[variant].input}`}
				value={value}
				onChange={onChange}
			/>
		</div>
	);
};

export default Form;
