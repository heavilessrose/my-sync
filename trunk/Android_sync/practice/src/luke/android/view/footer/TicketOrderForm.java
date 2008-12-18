//package luke.android.view.footer;
//
//import java.util.Calendar;
//
//import luke.android.R;
//
//import android.os.Bundle;
//import android.view.View;
//import android.widget.AdapterView;
//import android.widget.ArrayAdapter;
//import android.widget.DatePicker;
//import android.widget.EditText;
//import android.widget.LinearLayout;
//import android.widget.RadioGroup;
//import android.widget.Spinner;
//import android.widget.TextView;
//import android.app.Activity;
//
//public class TicketOrderForm extends Activity implements
//		RadioGroup.OnCheckedChangeListener, View.OnClickListener {
//	TextView mDateDisplay;
//	String price[];
//	private EditText et;
//	protected static RadioGroup mRadioGroup;
//	protected static int choice = -2;
//	private String strVal;
//	// date and time
//	private int mYear;
//	private int mMonth;
//	private int mDay;
//
//	@Override
//	protected void onCreate(Bundle icicle) {
//		super.onCreate(icicle);
//		setContentView(R.layout.Form_Design);
//		Bundle bndl = getIntent().getExtras();
//		ProcessRailway.biller = bndl.getCharSequence("item").toString();
//		Spinner sp = (Spinner) findViewById(R.id.spSrc);
//		addSpinner(sp);
//		sp = (Spinner) findViewById(R.id.spDest);
//		addSpinner(sp);
//
//		mRadioGroup = (RadioGroup) findViewById(R.id.group1);
//		mRadioGroup.setOnCheckedChangeListener(this);
//		et = (EditText) findViewById(R.id.Qty);
//		mDateDisplay = (TextView) findViewById(R.id.Date);
//		pr = ProcessRailway.getInstance();
//		Button pickDate = (Button) findViewById(R.id.btnDate);
//		pickDate.setOnClickListener(new View.OnClickListener() {
//
//			public void onClick(View v) {
//				new DatePickerDialog(TicketOrderForm.this, mDateSetListener,
//						mYear, mMonth, mDay, Calendar.SUNDAY).show();
//
//			}
//		});
//
//		final Calendar c = Calendar.getInstance();
//		mYear = c.get(Calendar.YEAR);
//		mMonth = c.get(Calendar.MONTH);
//		mDay = c.get(Calendar.DAY_OF_MONTH);
//		LinearLayout lay = (LinearLayout) findViewById(R.id.ticketId);
//		lay.addView(pr.getFooterView());
//	}
//
//	private void addSpinner(Spinner sp) {
//		ArrayAdapter<String> pizzaTyp = new ArrayAdapter<String>(this,
//				android.R.layout.simple_spinner_item, getResources()
//						.getStringArray(R.array.places));
//		pizzaTyp.setDropDownViewResource(R.layout.drop_down_item);
//		sp.setAdapter(pizzaTyp);
//		sp.setOnItemSelectedListener(spnListener);
//
//	}
//
//	private Spinner.OnItemSelectedListener spnListener = new Spinner.OnItemSelectedListener() {
//		public void onItemSelected(AdapterView parent, View v, int position,
//				long id) {
//
//		}
//
//		public void onNothingSelected(AdapterView arg0) {
//			// TODO Auto-generated method stub
//
//		}
//	};
//
//	private void updateDisplay() {
//		mDateDisplay.setText(new StringBuilder().append(pad(mMonth))
//				.append("/").append(pad(mDay)).append("/").append(mYear));
//
//	}
//
//	private DatePicker.OnDateSetListener mDateSetListener = new DatePicker.OnDateSetListener() {
//
//		public void dateSet(DatePicker view, int year, int monthOfYear,
//				int dayOfMonth) {
//			mYear = year;
//			mMonth = monthOfYear;
//			mDay = dayOfMonth;
//			updateDisplay();
//		}
//	};
//
//	public void onClick(View arg0) {
//		mRadioGroup.clearCheck();
//
//	}
//
//	public void onCheckedChanged(RadioGroup arg0, int checkedId) {
//
//		choice = checkedId;
//
//	}
//
//	@Override
//	public boolean onCreateOptionsMenu(Menu menu) {
//		super.onCreateOptionsMenu(menu);
//		menu.add(0, 0, getString(R.string.MENU_OK), R.drawable.ok);
//
//		return true;
//	}
//
//	public boolean onOptionsItemSelected(Menu.Item item) {
//
//		Intent in = new Intent();
//		switch (item.getId()) {
//		case 0:
//			try {
//				int qty = Integer.valueOf(et.getText().toString());
//				// int val=Integer.valueOf(strVal);
//				ProcessRailway.amt = String.valueOf(1000 * qty);
//				pr.initService(this);
//			} catch (NumberFormatException e) {
//				showAlert(null, 0, "Validation Failure",
//						"Please Check the Fields !!!", true);
//			}
//
//			break;
//		}
//		return false;
//	}
//
//	@Override
//	protected void onDestroy() {
//
//		super.onDestroy();
//
//	}
//
//	@Override
//	protected void onResume() {
//		// TODO Auto-generated method stub
//		super.onResume();
//		if (ProcessRailway.ppsStatus == true) {
//			ProcessRailway.ppsStatus = false;
//			try {
//				if (ProcessRailway.conn != null)
//					pr.releaseService(this);
//				finish();
//			} catch (Exception e) {
//				// TODO: handle exception
//			}
//		}
//
//	}
//
//	private static String pad(int c) {
//		if (c >= 10)
//			return String.valueOf(c);
//		else
//			return "0" + String.valueOf(c);
//	}
//
//	public View getFooterView() {
//
//		ViewInflate vif = (ViewInflate) mchekCtx
//				.getSystemService(Context.INFLATE_SERVICE);
//		View fv = vif.inflate(R.layout.footer, null, null);
//		LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
//				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//		lp.gravity = 0x01;
//		fv.setClickable(false);
//		fv.setSelected(false);
//		fv.setLayoutParams(lp);
//		return fv;
//	}
//
//}