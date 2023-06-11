/** @type {import('tailwindcss').Config} */
module.exports = {
	content: ['./src/**/*.{js,jsx,ts,tsx}'],
	theme: {
		extend: {
			fontFamily: {
				axiata: ['Axiata', 'sans-serif'],
				inter: ['Inter', 'sans-serif'],
			},
			colors: {
				primary: '#1D1E42',
				secondary: '#404086',
				tertiary: '#2F2F5C',
				bluish: '#30C9FF',
				pinkish: '#F51DAB',
				orangish: '#EA7061',
				purplish: '#C25DFF',
			},
		},
	},
	plugins: [],
};
