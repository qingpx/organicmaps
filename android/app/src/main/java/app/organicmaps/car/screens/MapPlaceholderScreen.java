package app.organicmaps.car.screens;

import androidx.annotation.NonNull;
import androidx.car.app.CarContext;
import androidx.car.app.model.Action;
import androidx.car.app.model.CarIcon;
import androidx.car.app.model.MessageTemplate;
import androidx.car.app.model.Template;
import androidx.core.graphics.drawable.IconCompat;

import app.organicmaps.R;
import app.organicmaps.car.screens.base.BaseScreen;
import app.organicmaps.display.DisplayManager;
import app.organicmaps.display.DisplayType;

public class MapPlaceholderScreen extends BaseScreen
{
  public MapPlaceholderScreen(@NonNull CarContext carContext)
  {
    super(carContext);
  }

  @NonNull
  @Override
  public Template onGetTemplate()
  {
    final MessageTemplate.Builder builder = new MessageTemplate.Builder(getCarContext().getString(R.string.aa_used_on_the_phone_screen));
    builder.setHeaderAction(Action.APP_ICON);
    builder.setTitle(getCarContext().getString(R.string.app_name));
    builder.setIcon(new CarIcon.Builder(IconCompat.createWithResource(getCarContext(), R.drawable.ic_phone_android)).build());
    builder.addAction(new Action.Builder().setTitle(getCarContext().getString(R.string.aa_continue_in_the_car))
        .setOnClickListener(() -> DisplayManager.from(getCarContext()).changeDisplay(DisplayType.Car)).build());

    return builder.build();
  }
}
