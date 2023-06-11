import React from 'react';

const Button = ({ children, className, onClick }) => {
	return (
		<button
            type ="submit"
			className={`h-8 rounded-full text-sm tracking-widest text-bluish ${className}`}
			onClick={onClick}>
			{children}
		</button>
	);
};

export default Button;
